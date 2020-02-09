/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Writer.h>
#include <logbook/Logbook.h>
#include <mutex>

namespace logbook {

/* Defined in Logbook.cpp */
void releaseWriter(Writer& writer);

Writer::Writer(Writer&& writer)
: location(writer.location),
  oStream(writer.oStream)
{
	writer.oStream = nullptr;
}

Writer::Writer(Location* aLocation, std::ostream* aOStream)
: location(aLocation),
  oStream(aOStream)
{ }

Writer::Writer()
: location(nullptr),
  oStream(nullptr)
{ }

Writer::~Writer() {
    if(oStream) {
    	oStream->flush();
        releaseWriter(*this);
    }
}

Writer& Writer::operator<<(std::ostream& (*pf)(std::ostream&)) {
    if(oStream) {
    	(*oStream) << pf;
    }
    return *this;
}

} /* namespace logbook */
