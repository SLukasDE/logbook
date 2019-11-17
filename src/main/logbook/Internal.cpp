/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#include <logbook/Internal.h>
#include <logbook/Logger.h>

namespace logbook {

unsigned int Internal::getThreadNo(std::thread::id threadId) {
	return Logger::getThreadNo(threadId);
}

std::pair<Id*, std::reference_wrapper<std::ostream>> Internal::pushCurrent(Id id, bool** enabled) {
	return Logger::pushCurrent(id, enabled);
}

void Internal::popCurrent() {
	Logger::popCurrent();
}

void Internal::popCurrent(std::stringstream& sstream, Id& id) {
	Logger::popCurrent(sstream, id);
}

} /* namespace logbook */
