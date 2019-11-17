/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_STREAM_H_
#define LOGBOOK_STREAM_H_

#include <logbook/Level.h>
#include <logbook/Writer.h>

namespace logbook {

class Stream {
public:
	Stream(const char* typeName, Level ll);
	~Stream() = default;

    Writer operator()(void* object);
    Writer operator()(const char* function, const char* file, unsigned int lineNo);
    Writer operator()(void* object, const char* function, const char* file, unsigned int lineNo);

	template<typename T>
	inline Writer operator<<(const T& t) {
        Writer writer(getWriter(nullptr, nullptr, nullptr, 0));
        writer << t;
        return writer;
	}

    Writer operator<<(std::ostream& (*pf)(std::ostream&));

private:
    Writer getWriter(void* object, const char* function, const char* file, unsigned int lineNo);

    bool* enabled = nullptr;
    const char* typeName;
	Level level;
};

} /* namespace logbook */

#endif /* LOGBOOK_STREAM_H_ */
