/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Writer.h>
#include <logbook/Logger.h>
#include <mutex>

namespace logbook {

Writer::Writer(Id* aId, std::ostream& aOStream)
: doUnlock(true),
  id(aId),
  oStream(aOStream)
{
}

Writer::Writer(Writer&& writer)
: doUnlock(writer.doUnlock),
  id(writer.id),
  oStream(writer.oStream)
{
	writer.doUnlock = false;
}

Writer::~Writer() {
    if(doUnlock) {
    	oStream.flush();

    	if(id == nullptr) {
            Logger::popCurrent();
    	}
    	else {
            Logger::popCurrent(static_cast<std::stringstream&>(oStream), *id);
    	}
    }
}

Writer& Writer::operator<<(std::ostream& (*pf)(std::ostream&)) {
	oStream << pf;
    return *this;
}

} /* namespace logbook */
