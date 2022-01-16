/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Logbook.h>
#include <logbook/Appender.h>
#include <logbook/Location.h>
#include <logbook/StreamBuffer.h>

#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility> // std::pair
#include <functional> //template< class T > struct hash<T*>; std::reference_wrapper
#include <mutex>
#include <sstream>

namespace logbook {

namespace {  /* anonymous namespace */

struct EnabledLevel {
	bool trace;
	bool debug;
	bool info;
	bool warn;
	bool error;

	void setLogLevel(Level logLevel) {
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
};



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

} /* anonymous namespace */

/* must be out of anonymous namespace because class (or at least member createWriter(...) ) must be friend of Writer */

class Logbook {
public:
	Logbook()
	: streamBuffer(1000),
	  oStream(&streamBuffer),
	  currentLocation(Level::SILENT, nullptr, nullptr, nullptr, nullptr, 0, std::this_thread::get_id())
	{ }

	~Logbook() {
		flush();
	}

	// NOT thread save - call it at the beginning if needed. Default is already "true"
	// unblocked behavior makes other threads not waiting on logging, while current thread is writing to logger already.
	// If logger is used already by current thread, other threads will write to a temporary buffer.
	// - Temporary buffer is flushed to real logger, if other thread is done using the logger.
	// - If logger is still used by current thread, buffer is queued.
	// - If current thread is done using the logger, it flushes queued buffers.
	void setUnblocked(bool aIsUnblocked) {
		isUnblocked = aIsUnblocked;
	}

	// thread safe, quaranteed by configMutex
	/*
	setLogLevel:
	1.1 remove first 2 characters "::" in typeName if exists
	1.2 make entry in "typeNameToLogLevel"
	2. Set default log level if typeName is ""
	3. iterate over "typeNameToWriters"
	3.1 find for each entry the most specific logLevel-Definition in "typeNameToLogLevel":
	3.2 Set writers for this entry according to found logLevel
	*/
	void setLevel(Level logLevel, const std::string& aTypeName) {
		// corrected aTypeName and store corrected version to typeName
		// Makes aTypeName = ":::::abc---XYZ" to typeName = "abc---XYZ"
	    std::string typeName = removeStartingCharactersIfMatch(aTypeName , [](char c) { return c == ':'; });

	    std::lock_guard<std::mutex> configLock(configMutex);
	    typeNameToLogLevel[typeName] = logLevel;

	    for(auto& typeNameEnabledLevel : typeNameToEnabledLevel) {
	    	Level level = findMostSpecificLevelEntryByTypeName(typeNameEnabledLevel.first);
	    	typeNameEnabledLevel.second.setLogLevel(level);
	    }

	    /* reset currentLocation */
	    currentLocation = Location(Level::SILENT, nullptr, nullptr, nullptr, nullptr, 0, std::this_thread::get_id());
	}

	// thread safe, quaranteed by loggerMutex
	void addAppender(Appender& appender) {
	    std::lock_guard<std::recursive_mutex> loggerLock(loggerMutex);

		appenders.insert(&appender);
	}

	// thread safe, quaranteed by loggerMutex
	void removeAppender(Appender& appender) {
	    std::lock_guard<std::recursive_mutex> loggerLock(loggerMutex);

		appenders.erase(&appender);
	}

	unsigned int getThreadNo(std::thread::id threadId) {
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

	std::unique_ptr<Writer> createWriter(const Location& location) {
		// check if pointer has been set already. So we call this expensive function only once.
		bool isOwner = true;
		if(isUnblocked) {
			isOwner = loggerMutex.try_lock();
		}
		else {
			loggerMutex.lock();
		}

		/* there is another thread using the logger right now */
		if(!isOwner) {
			Location* locationPtr = new Location(location);
			locationPtr->enabled = isLoggingEnabled(location.typeName, location.level);
			return std::unique_ptr<Writer>(new Writer(locationPtr, static_cast<std::ostream*>(new std::stringstream)));
		}

		/* there is no other thread using the logger right now */

		if(currentLocation != location) {
			// to create a number, if not done before - important for later translation of same threadId again, e.g. for appender MemBuffer
			getThreadNo(location.threadId);

			currentLocation = location;
			currentLocation.enabled = isLoggingEnabled(location.typeName, location.level);
		}

		return std::unique_ptr<Writer>(new Writer(nullptr, &oStream));
	}

	/* This function is called by Writer::~Writer() */
	void releaseWriter(Writer& writer) {
		bool isOwner = (writer.location == nullptr);

		if(isOwner == false) {
		    std::stringstream* sStream = static_cast<std::stringstream*>(writer.oStream);
			/* first be push the stringstream buffer to a queue */
			if(sStream) {
			    std::lock_guard<std::mutex> configLock(configMutex);
				unblockedBuffer.push_back(std::make_pair(writer.location, sStream->str()));
				delete sStream;
			}

			/* check if direct output is not in use anymore.
			 * Then we can flush the unblocked buffer output.
			 */
			isOwner = loggerMutex.try_lock();
		}

		if(isOwner) {
			flushBuffer();
			loggerMutex.unlock();
		}
	}

	// thread safe, quaranteed by configMutex
	/* isLoggingEnabled tells if the given log level is enabled for the given type name.
	 * Internally it looks the configMutex to access typeNameToEnabledLevel and
	 * adds an entry if not exists already for the given type name.
	 * This function always updates the entry
	 */
	bool isLoggingEnabled(const char* typeName, Level level) {
		// corrected typeName and store corrected version to myTypeName
	    std::string myTypeName = removeStartingCharactersIfMatch(typeName, [](char c) { return c == ':'; });

	    std::lock_guard<std::mutex> configLock(configMutex);
		// Find or CREATE(!) entry for given typeName (or for corrected version myTypeName)
	    EnabledLevel& enabledLevel = typeNameToEnabledLevel[myTypeName];

	    // Update this entry !
	    enabledLevel.setLogLevel(findMostSpecificLevelEntryByTypeName(myTypeName));

	    switch(level) {
	    case Level::TRACE:
	    	return enabledLevel.trace;
	    case Level::DEBUG:
	    	return enabledLevel.debug;
	    case Level::INFO:
	    	return enabledLevel.info;
	    case Level::WARN:
	    	return enabledLevel.warn;
	    //case Level::ERROR:
	    default:
	    	break;
	    }
		return enabledLevel.error;
	}

	void write(const char* ptr, std::size_t size) {
		for(auto& appender : appenders) {
			appender->write(currentLocation, ptr, size);
		}
	}

	void flush() {
		for(auto& appender : appenders) {
			appender->flush();
		}
	}

private:
	StreamBuffer streamBuffer;
	std::ostream oStream;

	std::recursive_mutex loggerMutex;
	std::unordered_map<std::thread::id, unsigned int> threadIdToNo;
	std::unordered_set<Appender*> appenders;

	Location currentLocation;

	// Lock for typeNameToLogLevel, typeNameToEnabledLevel and unblockedBuffer
	std::mutex configMutex;

	// type name with optional ending wildcard '*' -> logLevel, wildcard only allowed at the end!
	// must be locked by configMutex
	std::map<std::string, Level> typeNameToLogLevel;

	// type name without wildcard -> EnabledLevel
	// must be locked by configMutex
	std::map<std::string, EnabledLevel> typeNameToEnabledLevel;

	// must be locked by configMutex
	std::vector<std::pair<Location*, std::string>> unblockedBuffer;

	bool isUnblocked = true;

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
			// 1. loop: returns typeName = "AAA::BBB::"
			// 2. loop: returns typeName = "AAA::"
			// 3. loop: returns typeName = ""

			// 1. loop: find typeName = "AAA::BBB::*"
			// 2. loop: find typeName = "AAA::*"
			// 3. loop: find typeName = "*"
			iter = typeNameToLogLevel.find(typeName+'*');
			if(iter != std::end(typeNameToLogLevel)) {
				return iter->second;
			}

			// not found!
			typeName = removeEndingCharactersIfMatch(std::move(typeName), [](char c) { return c == ':'; });
			// 1. loop: returns typeName = "AAA::BBB"
			// 2. loop: returns typeName = "AAA"
			// 3. loop: returns typeName = ""
		}

		iter = typeNameToLogLevel.find("*");
		if(iter != std::end(typeNameToLogLevel)) {
			return iter->second;
		}

		constexpr Level defaultLogLevel = Level::INFO;
		typeNameToLogLevel["*"] = defaultLogLevel;
		return defaultLogLevel;
	}

	void flushBuffer() {
	    std::lock_guard<std::mutex> configLock(configMutex);

		for(const auto& item : unblockedBuffer) {
			// to create a number, if not done before - important for later translation of same threadId again, e.g. for appender MemBuffer
			getThreadNo(item.first->threadId);

//			if(currentLocation != *item.first) {
//				flushNewline();
//			}

			currentLocation = std::move(*item.first);
			currentLocation.enabled = item.first->enabled;

			write(item.second.data(), item.second.size());
			delete item.first;
		}
		unblockedBuffer.clear();
	}
};

namespace {  /* anonymous namespace */

typename std::aligned_storage<sizeof(Logbook), alignof(Logbook)>::type logbookBuffer; // memory for the object;
Logbook* logbookPtr = nullptr;

bool isDeleted = false;

struct Initializer {
	~Initializer() {
		if(logbookPtr) {
			logbookPtr->~Logbook();
			isDeleted = true;
		}
	}
} initializer;

Logbook* getLogbook() {
	if(isDeleted) {
		return nullptr;
	}

	if(logbookPtr == nullptr) {
		/* ***************** *
		 * initialize module *
		 * ***************** */

		logbookPtr = reinterpret_cast<Logbook*> (&logbookBuffer);
		new (logbookPtr) Logbook; // placement new
	}

	return logbookPtr;
}

}  /* anonymous namespace */

void setUnblocked(bool isUnblocked) {
	if(getLogbook()) {
		getLogbook()->setUnblocked(isUnblocked);
	}
}

void setLevel(Level logLevel, const std::string& typeName) {
	if(getLogbook()) {
		getLogbook()->setLevel(logLevel, typeName);
	}
}

/* Only used in Appender.cpp.
 * Function is thread safe, quaranteed by loggerMutex. */
void addAppender(Appender& appender) {
	if(getLogbook()) {
		getLogbook()->addAppender(appender);
	}
}

/* Only used in Appender.cpp.
 * Function is thread safe, quaranteed by loggerMutex. */
void removeAppender(Appender& appender) {
	if(getLogbook()) {
		getLogbook()->removeAppender(appender);
	}
}

unsigned int getThreadNo(std::thread::id threadId) {
	if(getLogbook()) {
		return getLogbook()->getThreadNo(threadId);
	}
	return 0;
}

std::unique_ptr<Writer> createWriter(const Location& location) {
	if(getLogbook()) {
		return getLogbook()->createWriter(location);
	}
	return nullptr;
}

void releaseWriter(Writer& writer) {
	if(getLogbook()) {
		getLogbook()->releaseWriter(writer);
	}
}

bool isLoggingEnabled(const char* typeName, Level level) {
	if(getLogbook()) {
		return getLogbook()->isLoggingEnabled(typeName, level);
	}
	return false;
}

void flush() {
	if(getLogbook()) {
		getLogbook()->flush();
	}
}

void write(const char* ptr, std::size_t size) {
	if(getLogbook()) {
		getLogbook()->write(ptr, size);
	}
}

} /* namespace logbook */
