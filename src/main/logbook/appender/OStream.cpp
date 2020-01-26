/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/appender/OStream.h>
#include <logbook/Logger.h>
#include <iostream>
#include <cstdio>

namespace logbook {
namespace appender {


OStream::OStream(std::ostream& oStreamTrace,
		std::ostream& oStreamDebug,
		std::ostream& oStreamInfo,
		std::ostream& oStreamWarn,
		std::ostream& oStreamError)
: Appender(),
  oStreamTrace(oStreamTrace),
  oStreamDebug(oStreamDebug),
  oStreamInfo(oStreamInfo),
  oStreamWarn(oStreamWarn),
  oStreamError(oStreamError)
{
}

void OStream::flushNewLine(const Location& location, bool enabled) {
	switch(getRecordLevel()) {
	case RecordLevel::OFF:
		return;
	case RecordLevel::ALL:
		break;
	default: /* RecordLevel::SELECTED */
		if(!enabled) {
			return;
		}
		break;
	}

	std::ostream& oStream = getOStream(location.level);
	if(!isFirstCharacterInLine) {
		oStream << "\n";
		isFirstCharacterInLine = true;
	}
	oStream.flush();
}

void OStream::write(const Location& location, bool enabled, const char* ptr, std::size_t size) {
	switch(getRecordLevel()) {
	case RecordLevel::OFF:
		return;
	case RecordLevel::ALL:
		break;
	default: /* RecordLevel::SELECTED */
		if(!enabled) {
			return;
		}
		break;
	}

	std::ostream& oStream = getOStream(location.level);
	const char* begin = ptr;

	for(auto iter = ptr; iter != &ptr[size]; ++iter) {
		if(isFirstCharacterInLine) {
			oStream << getLayout().makePrefix(location);
			isFirstCharacterInLine = false;
		}

		if(*iter == '\n') {
			oStream << std::string(begin, iter) << "\n";
			isFirstCharacterInLine = true;
			begin = iter+1;
		}
	}
	oStream << std::string(begin, &ptr[size]);
}

std::ostream& OStream::getOStream(Level level) {
	switch(level) {
	case Level::TRACE:
		return oStreamTrace;
	case Level::DEBUG:
		return oStreamDebug;
	case Level::INFO:
		return oStreamInfo;
	case Level::WARN:
		return oStreamWarn;
	case Level::ERROR:
		return oStreamError;
	default:
		break;
	}

	return oStreamError;
}

} /* namespace appender */
} /* namespace logbook */
