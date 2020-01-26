/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LOGGER_H_
#define LOGBOOK_LOGGER_H_

#include <logbook/Level.h>
#include <logbook/Location.h>
#include <logbook/Stream.h>
#include <logbook/Buffer.h>
#include <logbook/Internal.h>

#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <mutex>
#include <string>
#include <memory>
#include <functional> // std::reference_wrapper<>
#include <thread>

namespace logbook {

class Appender;

class Logger {
public:
	/* to call pushCurrent and access oStream */
	friend class Stream;

	/* to call popCurrent */
	friend class Writer;

	/* to call getThreadNo */
	friend class Layout;

	/* to call pushCurrent, popCurrent, getThreadNo and access oStream */
	friend class Internal;

	Logger(const char* typeName = "");

	// NOT thread save - call it at the beginning if needed. Default is already "true"
	// unblocked behavior makes other threads not waiting on logging, while current thread is writing to logger already.
	// If logger is used already by current thread, other threads will write to a temporary buffer.
	// - Temporary buffer is flushed to real logger, if other thread is done using the logger.
	// - If logger is still used by current thread, buffer is queued.
	// - If current thread is done using the logger, it flushes queued buffers.
	static void setUnblocked(bool isUnblocked);

	// thread safe, quaranteed by configMutex
	void setLevel(Level logLevel);
	static void setLevel(Level logLevel, const std::string& typeName);

	// thread safe, quaranteed by loggerMutex
	static void addAppender(Appender& appender);
	static std::vector<std::reference_wrapper<Appender>> getAppenders();

	Stream trace;
	Stream debug;
	Stream info;
	Stream warn;
	Stream error;

private:
	static unsigned int getThreadNo(std::thread::id threadId);

	static std::pair<Location*, std::reference_wrapper<std::ostream>> pushCurrent(Location location, bool** enabled);
	static void popCurrent();
	static void popCurrent(std::stringstream& sstream, Location& location);

	// createEnabledSwitch is only called indirectly by StreamReal::getStreamWriter where mutex gets locked
	static bool& createEnabledSwitch(const char* typeName, Level level);
/*
	static void setCurrent(Id id, bool enabled);
	static std::mutex& getMutex();
*/
	static void flushBuffer();
	static void flushNewline();
	static void write(const char* ptr, std::size_t size);

	const char* typeName;
	static Buffer streamBuffer;
	static std::ostream oStream;
};

extern Logger logger;

} /* namespace logbook */

#endif /* LOGBOOK_LOGGER_H_ */
