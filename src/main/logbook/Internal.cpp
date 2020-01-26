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

bool& Internal::isEnabled(const char* typeName, Level level) {
	return Logger::createEnabledSwitch(typeName, level);
}

std::pair<Location*, std::reference_wrapper<std::ostream>> Internal::pushCurrent(Location location, bool** enabled) {
	return Logger::pushCurrent(location, enabled);
}

void Internal::popCurrent() {
	Logger::popCurrent();
}

void Internal::popCurrent(std::stringstream& sstream, Location& location) {
	Logger::popCurrent(sstream, location);
}

} /* namespace logbook */
