/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/layout/Default.h>
#include <logbook/Level.h>
#include <logbook/Logbook.h>
#include <time.h>
#include <ctime>

namespace logbook {
namespace layout {

namespace {
std::string formatStrToSize(std::string str, bool spacesAtLeftSide = false, const std::size_t strSize = 50) {
	if(str.size() > strSize) {
		str = "..." + str.substr(str.size()+3-strSize);
	}

	if(spacesAtLeftSide) {
		while(str.size() < strSize) {
			str = " " + str;
		}
	}
	else {
		while(str.size() < strSize) {
			str += " ";
		}
	}

	return str;
}

std::string makeString(const char* typeName) {
	return typeName == nullptr ? "" : typeName;
}

std::string formatTimestamp(const std::time_t& timestamp) {
    char timeStr[23];
    struct tm timeBuf;
    struct tm* timePtr;

    timePtr = localtime_r(&timestamp, &timeBuf);
    sprintf(timeStr, "$ %04d-%02d-%02d %02d:%02d:%02d ",
            timePtr->tm_year + 1900,
            timePtr->tm_mon  + 1,
            timePtr->tm_mday,
            timePtr->tm_hour,
            timePtr->tm_min,
            timePtr->tm_sec);
    return timeStr;
}

std::string formatLevel(Level level) {
    switch(level) {
    case Level::TRACE:
    	return "[TRACE] ";
    case Level::DEBUG:
    	return "[DEBUG] ";
    case Level::INFO:
    	return "[INFO ] ";
    case Level::WARN:
    	return"[WARN ] ";
    case Level::ERROR:
    	return "[ERROR] ";
    default:
        break;
    }
	return "[ n/a ] ";
}

std::string formatTypeName(const char* typeName) {
	return formatStrToSize(makeString(typeName), false, 20);
}

std::string formatThreadNo(unsigned int threadNo) {
	return formatStrToSize(std::to_string(threadNo),true, 3);
}

std::string formatObject(const void* object) {
	char buffer[20];
	std::snprintf(buffer, 20, "%p", object);
	return formatStrToSize(buffer, false, 18);
}

std::string formatLineNo(unsigned int lineNo) {
	return formatStrToSize(std::to_string(lineNo),true, 6);
}

} /* anonymous namespace */

std::string Default::toString(const Location& location) const {
	std::string rv;

	if(showTimestamp) {
		rv += formatTimestamp(location.timestamp);
	}

	if(showLevel) {
		rv += formatLevel(location.level);
	}

    rv += "(";
	if(showTypeName) {
	    rv += formatTypeName(location.typeName);
	}
	if(showThreadNo) {
		rv += "-" + formatThreadNo(getThreadNo(location.threadId));
	}
	if(showAddress) {
		rv += " @ " + formatObject(location.object);
	}
	if(showFunction) {
		rv += "|" + formatStrToSize(makeString(location.function), false, 20);
	}
	if(showFile) {
		rv += "|" + formatStrToSize(makeString(location.file), false, 20);
	}
	if(showLineNo) {
		rv += "|" + formatLineNo(location.line);
	}
	rv += "): ";

	return rv;
}

bool Default::getShowTimestamp() const {
	return showTimestamp;
}

void Default::setShowTimestamp(bool aShowTimestamp) {
	showTimestamp = aShowTimestamp;
}

bool Default::getShowLevel() const {
	return showLevel;
}

void Default::setShowLevel(bool aShowLevel) {
	showLevel = aShowLevel;
}

bool Default::getShowTypeName() const {
	return showTypeName;
}

void Default::setShowTypeName(bool aShowTypeName) {
	showTypeName = aShowTypeName;
}

bool Default::getShowAddress() const {
	return showAddress;
}

void Default::setShowAddress(bool aShowAddress) {
	showAddress = aShowAddress;
}

bool Default::getShowFile() const {
	return showFile;
}

void Default::setShowFile(bool aShowFile) {
	showFile = aShowFile;
}

bool Default::getShowFunction() const {
	return showFunction;
}

void Default::setShowFunction(bool aShowFunction) {
	showFunction = aShowFunction;
}

bool Default::getShowLineNo() const {
	return showLineNo;
}

void Default::setShowLineNo(bool aShowLineNo) {
	showLineNo = aShowLineNo;
}

bool Default::getShowThreadNo() const {
	return showThreadNo;
}

void Default::setShowThreadNo(bool aShowThreadNo) {
	showThreadNo = aShowThreadNo;
}

} /* namespace layout */
} /* namespace logbook */
