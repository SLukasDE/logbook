/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_STREAM_H_
#define LOGBOOK_STREAM_H_

#include <logbook/Level.h>
#include <logbook/Writer.h>
#include <logbook/Logbook.h>

namespace logbook {
inline namespace v0_4 {

class Stream {
public:
	Stream(const char* typeName, Level ll);
	~Stream() = default;

    Writer operator()(void* object);
    Writer operator()(const char* function, const char* file, unsigned int lineNo);
    Writer operator()(void* object, const char* function, const char* file, unsigned int lineNo);

	template<typename T>
	inline Writer operator<<(const T& t) {
		std::unique_ptr<Writer> writerPtr(createWriter(Location(level, nullptr, typeName, nullptr, nullptr, 0, std::this_thread::get_id())));
		if(writerPtr) {
	    	Writer writer(std::move(*writerPtr));
	        writer << t;
	        return writer;
		}
    	return Writer();
	}

    Writer operator<<(std::ostream& (*pf)(std::ostream&));

private:
    const char* typeName;
	Level level;
};

} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_STREAM_H_ */
