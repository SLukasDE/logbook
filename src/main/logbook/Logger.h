/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LOGGER_H_
#define LOGBOOK_LOGGER_H_

#include <logbook/Level.h>
//#include <logbook/Location.h>
#include <logbook/Stream.h>
/*
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
*/
namespace logbook {

class Appender;

class Logger {
public:
#if 0
	/* to call pushCurrent and access oStream */
	friend class Stream;

	/* to call popCurrent */
	friend class Writer;

	/* to call getThreadNo */
	friend class Layout;

	/* to call pushCurrent, popCurrent, getThreadNo and access oStream */
	friend class Internal;
#endif
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

} /* namespace logbook */

#endif /* LOGBOOK_LOGGER_H_ */
