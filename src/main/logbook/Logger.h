/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LOGGER_H_
#define LOGBOOK_LOGGER_H_

#include <logbook/Level.h>
#include <logbook/Stream.h>

namespace logbook {
inline namespace v0_4 {

class Appender;

class Logger {
public:
	Logger(const char* typeName = "");

	// thread safe, quaranteed by configMutex
	void setLevel(Level logLevel);

	Stream trace;
	Stream debug;
	Stream info;
	Stream warn;
	Stream error;

private:
	const char* typeName;
};

extern Logger logger;

} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_LOGGER_H_ */
