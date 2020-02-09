/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Logger.h>
#include <logbook/Logbook.h>
/*
#include <map>
#include <string>
#include <unordered_map>
*/
namespace logbook {

namespace {
#if 0
struct Context {
std::recursive_mutex loggerMutex;
std::unordered_map<std::thread::id, unsigned int> threadIdToNo;
std::vector<std::reference_wrapper<Appender>> appenders;

Location currentLocation(Level::SILENT, nullptr, nullptr, nullptr, nullptr, 0, std::this_thread::get_id());

// Lock for typeNameToLogLevel, typeNameToEnabledLevel and unblockedBuffer
std::mutex configMutex;

// type name with optional ending wildcard '*' -> logLevel, wildcard only allowed at the end!
// must be locked by configMutex
std::map<std::string, Level> typeNameToLogLevel;

// type name without wildcard -> EnabledLevel
// must be locked by configMutex
std::map<std::string, EnabledLevel> typeNameToEnabledLevel;

// must be locked by configMutex
std::vector<std::pair<Location*, std::stringstream*>> unblockedBuffer;

bool isUnblocked = true;
};

typename std::aligned_storage<sizeof(Context), alignof(Context)>::type contextBuffer; // memory for the object;
Context* contextPtr = nullptr;

Context& getContext() {
	if(contextPtr == nullptr) {
		contextPtr = reinterpret_cast<Context*> (&contextBuffer);
		new (contextPtr) Context(); // placement new
	}
	return *contextPtr;
}
#endif

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

void Logger::setLevel(Level logLevel) {
	logbook::setLevel(logLevel, typeName);
}

logbook::Logger logger("logbook");

} /* namespace logbook */


