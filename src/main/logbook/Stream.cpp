/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Stream.h>
//#include <logbook/Location.h>

#include <iostream>

namespace logbook {

Stream::Stream(const char* typeName, Level ll)
: typeName(typeName),
  level(ll)
{ }

Writer Stream::operator()(void* object) {
	std::unique_ptr<Writer> writerPtr(createWriter(Location(level, object, typeName, nullptr, nullptr, 0, std::this_thread::get_id())));
	if(writerPtr) {
    	Writer writer(std::move(*writerPtr));
        return writer;
	}
	return Writer();
}

Writer Stream::operator<<(std::ostream& (*pf)(std::ostream&)) {
	std::unique_ptr<Writer> writerPtr(createWriter(Location(level, nullptr, typeName, nullptr, nullptr, 0, std::this_thread::get_id())));
	if(writerPtr) {
    	Writer writer(std::move(*writerPtr));
        writer << pf;
        return writer;
	}
	return Writer();
}

Writer Stream::operator()(const char* function, const char* file, unsigned int lineNo) {
	std::unique_ptr<Writer> writerPtr(createWriter(Location(level, nullptr, typeName, function, file, lineNo, std::this_thread::get_id())));
	if(writerPtr) {
    	Writer writer(std::move(*writerPtr));
        return writer;
	}
	return Writer();
}

Writer Stream::operator()(void* object, const char* function, const char* file, unsigned int lineNo) {
	std::unique_ptr<Writer> writerPtr(createWriter(Location(level, object, typeName, function, file, lineNo, std::this_thread::get_id())));
	if(writerPtr) {
    	Writer writer(std::move(*writerPtr));
        return writer;
	}
	return Writer();
}

} /* namespace logbook */
