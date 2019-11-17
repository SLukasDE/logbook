/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Logger.h>
#include <logbook/Appender.h>
#include <map>
#include <string>
#include <unordered_map>

namespace logbook {

namespace {
struct EnabledLevel {
	bool trace;
	bool debug;
	bool info;
	bool warn;
	bool error;

	void setLogLevel(Level logLevel);
};

void EnabledLevel::setLogLevel(Level logLevel) {
	switch(logLevel) {
	case Level::TRACE:
		trace = true;
		debug = true;
		info = true;
		warn = true;
		error = true;
		break;
	case Level::DEBUG:
		trace = false;
		debug = true;
		info = true;
		warn = true;
		error = true;
		break;
	case Level::INFO:
		trace = false;
		debug = false;
		info = true;
		warn = true;
		error = true;
		break;
	case Level::WARN:
		trace = false;
		debug = false;
		info = false;
		warn = true;
		error = true;
		break;
	case Level::ERROR:
		trace = false;
		debug = false;
		info = false;
		warn = false;
		error = true;
		break;
	case Level::SILENT:
		trace = false;
		debug = false;
		info = false;
		warn = false;
		error = false;
		break;
	default:
		break;
	}
}

std::recursive_mutex loggerMutex;
std::unordered_map<std::thread::id, unsigned int> threadIdToNo;
std::vector<std::reference_wrapper<Appender>> appenders;
std::vector<Id> currentStack;
Id current(Level::SILENT, nullptr, nullptr, nullptr, nullptr, 0, std::this_thread::get_id());
//bool currentEnabled = true;
//Id currentId(Level::SILENT, nullptr, nullptr, nullptr, nullptr, 0, std::this_thread::get_id());

std::mutex configMutex;
std::map<std::string, Level> typeNameToLogLevel; // type name with optional ending wildcard '*' -> logLevel, wildcard only allowed at the end!
std::map<std::string, EnabledLevel> typeNameToEnabledLevel; // type name without wildcard -> EnabledLevel
std::vector<std::pair<Id*, std::stringstream*>> unblockedBuffer;

bool isUnblocked = true;
Level defaultLogLevel = Level::INFO;

template<typename F>
std::string removeStartingCharactersIfMatch(std::string str, F f) {
	std::size_t i = 0;
	for(; i<str.size(); ++i) {
		if(f(str.at(i)) == false) {
			break;
		}
	}

	if(i > 0) {
		str.erase(0, i);
	}

	return str;
}

template<typename F>
std::string removeEndingCharactersIfMatch(std::string str, F f) {
	std::size_t i = str.size();
	for(; i>0; --i) {
		if(f(str.at(i-1)) == false) {
			break;
		}
	}
	if(i < str.size()) {
		str.erase(i, str.size() - i);
	}

	return str;
}

/*
1. find typeName in "typeNameToLogLevel"
2. if not found, replace last type in "typeNameToLogLevel" by wildcard (replace tailing "::[0-9a-zA-Z_]*" by "::*") and search again in "typeNameToLogLevel"
3. if not found and typeName != "*", then replace tailing "::[0-9a-zA-Z_]*::*" of typeName by "::*" and go back to 2.
   E.g. reduce "A::B::*" to "A::*"

Example: findMostSpecificLevelEntryByTypeName("A::B::C")
1. find definition for "AAA::BBB::CCC"
2. find definition for "AAA::BBB::*"
3. find definition for "AAA::*"
4. find definition for "*"
*/
Level findMostSpecificLevelEntryByTypeName(std::string typeName) {
	// typeName = "AAA::BBB::CCC"
	auto iter = typeNameToLogLevel.find(typeName);
	if(iter != std::end(typeNameToLogLevel)) {
		return iter->second;
	}

	while(!typeName.empty()) {
		// 1. loop: typeName = "AAA::BBB::CCC"
		// 2. loop: typeName = "AAA::BBB"
		// 3. loop: typeName = "AAA"

		typeName = removeEndingCharactersIfMatch(std::move(typeName), [](char c) { return c != ':'; });
		// 1. loop: typeName = "AAA::BBB::"
		// 2. loop: typeName = "AAA::"
		// 3. loop: typeName = ""

		iter = typeNameToLogLevel.find(typeName+'*');
		if(iter != std::end(typeNameToLogLevel)) {
			return iter->second;
		}

		typeName = removeEndingCharactersIfMatch(std::move(typeName), [](char c) { return c == ':'; });
		// 1. loop: typeName = "AAA::BBB"
		// 2. loop: typeName = "AAA"
		// 3. loop: typeName = ""
	}
	return defaultLogLevel;
}

} /* anonymous namespace */

Logger::Logger(const char* aTypeName)
: trace(aTypeName, Level::TRACE),
  debug(aTypeName, Level::DEBUG),
  info(aTypeName, Level::INFO),
  warn(aTypeName, Level::WARN),
  error(aTypeName, Level::ERROR),
  typeName(aTypeName)
{
}

void Logger::setUnblocked(bool aIsUnblocked) {
	isUnblocked = aIsUnblocked;
}

void Logger::setLevel(Level logLevel) {
	setLevel(logLevel, typeName);
}

/*
setLogLevel:
1.1 remove first 2 characters "::" in typeName if exists
1.2 make entry in "typeNameToLogLevel"
2. Set default log level if typeName is ""
3. iterate over "typeNameToWriters"
3.1 find for each entry the most specific logLevel-Definition in "typeNameToLogLevel":
3.2 Set writers for this entry according to found logLevel
*/

void Logger::setLevel(Level logLevel, const std::string& typeName) {
	// corrected typeName and store corrected version to myTypeName
    std::string myTypeName = removeStartingCharactersIfMatch(typeName, [](char c) { return c == ':'; });

    std::lock_guard<std::mutex> configLock(configMutex);
    typeNameToLogLevel[myTypeName] = logLevel;

    /* is it as well the default logLevel? */
    if(typeName == "*") {
    	defaultLogLevel = logLevel;
    }

    for(auto& typeNameEnabledLevel : typeNameToEnabledLevel) {
    	Level level = findMostSpecificLevelEntryByTypeName(typeNameEnabledLevel.first);
    	typeNameEnabledLevel.second.setLogLevel(level);
    }
}

void Logger::addAppender(Appender& appender) {
    std::lock_guard<std::recursive_mutex> loggerLock(loggerMutex);

	appenders.push_back(std::ref(appender));
}

std::vector<std::reference_wrapper<Appender>> Logger::getAppenders() {
	std::vector<std::reference_wrapper<Appender>> rv;

	{
	    std::lock_guard<std::recursive_mutex> loggerLock(loggerMutex);
		for(auto& appender : appenders) {
			rv.push_back(appender);
		}
	}

	return rv;
}

unsigned int Logger::getThreadNo(std::thread::id threadId) {
	static std::unordered_map<std::thread::id, unsigned int> threadIds;

	auto iter = threadIds.find(threadId);
	unsigned int id = threadIds.size()+1;
	if(iter == std::end(threadIds)) {
		threadIds[threadId] = id;
	}
	else {
		id = iter->second;
	}

	return id;
}

std::pair<Id*, std::reference_wrapper<std::ostream>> Logger::pushCurrent(Id id, bool** enabled) {
	if((*enabled) == nullptr) {
	    *enabled = Logger::createEnabledSwitch(id.typeName, id.level);
	}

	bool isOwner = true;
	if(isUnblocked) {
		isOwner = loggerMutex.try_lock();
	}
	else {
		loggerMutex.lock();
	}

	if(isOwner) {
		if(current == id) {
			return std::make_pair(nullptr, std::ref(oStream));
		}

		flushNewline();

		currentStack.push_back(std::move(current));

		// to create a number, if not done before - important for later translation of same threadId again, e.g. for appender MemBuffer
		getThreadNo(id.threadId);

		current = std::move(id);
		current.enabled = **enabled;

		return std::make_pair(nullptr, std::ref(oStream));
	}

	Id* idPtr = new Id(std::move(id));
	idPtr->enabled = **enabled;
    return std::make_pair(idPtr, std::ref(static_cast<std::ostream&>(*new std::stringstream)));
}

void Logger::popCurrent() {
	flushBuffer();

	current = std::move(currentStack.back());
	currentStack.pop_back();

	flushNewline();
	loggerMutex.unlock();
}

void Logger::popCurrent(std::stringstream& sstream, Id& id) {
	{
	    std::lock_guard<std::mutex> configLock(configMutex);
		unblockedBuffer.push_back(std::make_pair(&id, &sstream));
	}

	// now let's try to get the lock
	bool isOwner = loggerMutex.try_lock();
	if(isOwner == false) {
		return;
	}

	currentStack.push_back(std::move(current));

	popCurrent();
}

// only called indirectly by StreamReam::createWriters. Mutex is locked already
bool* Logger::createEnabledSwitch(const char* typeName, Level level) {
	// corrected typeName and store corrected version to myTypeName
    std::string myTypeName = removeStartingCharactersIfMatch(typeName, [](char c) { return c == ':'; });

    std::lock_guard<std::mutex> configLock(configMutex);
	// Find or CREATE(!) entry for given typeName (or for corrected version myTypeName)
    EnabledLevel& enabledLevel = typeNameToEnabledLevel[myTypeName];

    // Update this entry !
    enabledLevel.setLogLevel(findMostSpecificLevelEntryByTypeName(myTypeName));

    switch(level) {
    case Level::TRACE:
    	return &enabledLevel.trace;
    case Level::DEBUG:
    	return &enabledLevel.debug;
    case Level::INFO:
    	return &enabledLevel.info;
    case Level::WARN:
    	return &enabledLevel.warn;
    case Level::ERROR:
    	return &enabledLevel.error;
    default:
    	break;
    }
	return nullptr;
}

void Logger::flushBuffer() {
    std::lock_guard<std::mutex> configLock(configMutex);

	for(const auto& item : unblockedBuffer) {
		Logger::flushNewline();

		// to create a number, if not done before - important for later translation of same threadId again, e.g. for appender MemBuffer
		Logger::getThreadNo(item.first->threadId);

		current = std::move(*item.first);
		current.enabled = item.first->enabled;

		std::string str = item.second->str();
		Logger::write(str.data(), str.size());
		delete item.first;
		delete item.second;
	}
	unblockedBuffer.clear();
}

void Logger::flushNewline() {
	for(auto& appender : appenders) {
		appender.get().flushNewLine(current, current.enabled);
	}
}

void Logger::write(const char* ptr, std::size_t size) {
	for(auto& appender : appenders) {
		appender.get().write(current, current.enabled, ptr, size);
	}
}

Buffer Logger::streamBuffer(1000, Logger::write);
std::ostream Logger::oStream(&streamBuffer);

logbook::Logger logger("logbook");

} /* namespace logbook */


