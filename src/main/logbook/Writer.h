/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_STREAMWRITER_H_
#define LOGBOOK_STREAMWRITER_H_

#include <logbook/Location.h>
#include <ostream>

namespace logbook {

class Writer {
friend class Stream;
friend class Logbook;

public:
	Writer(const Writer&) = delete;
	~Writer();

	Writer& operator=(const Writer&) = delete;
	Writer& operator=(Writer&&) = delete;

	template<typename T2>
	inline Writer& operator<<(const T2& t2) {
		if(oStream) {
			(*oStream) << t2;
		}
		return *this;
	}

	Writer& operator<<(std::ostream& (*pf)(std::ostream&));

    std::ostream* getOStream() {
    	return oStream;
    }

private:
	Writer(Writer&& streamWriter);

	// just for Logbook::createWriter()
	Writer(Location* location, std::ostream* oStream);

	// just for Stream::operator<<(...) and Stream::operator()(...)
	Writer();

    Location* location;
    std::ostream* oStream;
};

} /* namespace logbook */

#endif /* LOGBOOK_STREAMWRITER_H_ */
