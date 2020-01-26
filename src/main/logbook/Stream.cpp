/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Stream.h>
#include <logbook/Logger.h>
#include <logbook/Location.h>

namespace logbook {

Stream::Stream(const char* typeName, Level ll)
: typeName(typeName),
  level(ll)
{ }

Writer Stream::operator()(void* object) {
    return Writer(getWriter(object, nullptr, nullptr, 0));
}

Writer Stream::operator<<(std::ostream& (*pf)(std::ostream&)) {
    Writer writer(getWriter(nullptr, nullptr, nullptr, 0));
    writer << pf;
    return writer;
}

Writer Stream::operator()(const char* function, const char* file, unsigned int lineNo) {
    return Writer(getWriter(nullptr, function, file, lineNo));
}

Writer Stream::operator()(void* object, const char* function, const char* file, unsigned int lineNo) {
    return Writer(getWriter(object, function, file, lineNo));
}

Writer Stream::getWriter(void* object, const char* function, const char* file, unsigned int lineNo) {
	std::pair<Location*, std::reference_wrapper<std::ostream>> rv = Logger::pushCurrent(Location(level, object, typeName, function, file, lineNo, std::this_thread::get_id()), &enabled);
	return Writer(rv.first, rv.second.get());
}

} /* namespace logbook */
