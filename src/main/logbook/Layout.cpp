/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Layout.h>
#include <logbook/Level.h>
#include <logbook/Logger.h>
#include <time.h>
#include <ctime>

namespace logbook {

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




bool Layout::getShowTimestamp() const {
	return showTimestamp;
}

void Layout::setShowTimestamp(bool showTimestamp) {
	this->showTimestamp = showTimestamp;
}

bool Layout::getShowLevel() const {
	return showLevel;
}

void Layout::setShowLevel(bool showLevel) {
	this->showLevel = showLevel;
}

bool Layout::getShowTypeName() const {
	return showTypeName;
}

void Layout::setShowTypeName(bool showTypeName) {
	this->showTypeName = showTypeName;
}

bool Layout::getShowAddress() const {
	return showAddress;
}

void Layout::setShowAddress(bool showAddress) {
	this->showAddress = showAddress;
}

bool Layout::getShowFile() const {
	return showFile;
}

void Layout::setShowFile(bool showFile) {
	this->showFile = showFile;
}

bool Layout::getShowFunction() const {
	return showFunction;
}

void Layout::setShowFunction(bool showFunction) {
	this->showFunction = showFunction;
}

bool Layout::getShowLineNo() const {
	return showLineNo;
}

void Layout::setShowLineNo(bool showLineNo) {
	this->showLineNo = showLineNo;
}

bool Layout::getShowThreadNo() const {
	return showThreadNo;
}

void Layout::setShowThreadNo(bool showThreadNo) {
	this->showThreadNo = showThreadNo;
}

std::string Layout::makePrefix(const Location& location) {
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
		rv += "-" + formatThreadNo(Logger::getThreadNo(location.threadId));
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

} /* namespace logbook */
