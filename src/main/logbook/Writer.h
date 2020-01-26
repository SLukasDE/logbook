/*
Copyright (c) 2019, Sven Lukas

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
public:
	Writer(const Writer&) = delete;
	~Writer();

	Writer& operator=(const Writer&) = delete;
	Writer& operator=(Writer&&) = delete;

	template<typename T2>
	inline Writer& operator<<(const T2& t2) {
		oStream << t2;
		return *this;
	}

	Writer& operator<<(std::ostream& (*pf)(std::ostream&));

private:
	Writer(Location* location, std::ostream& oStream);
	Writer(Writer&& streamWriter);

    bool doUnlock;
    Location* location;
    std::ostream& oStream;
};

} /* namespace logbook */

#endif /* LOGBOOK_STREAMWRITER_H_ */
