/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_INTERNAL_H_
#define LOGBOOK_INTERNAL_H_

#include <logbook/Id.h>
#include <utility>
#include <ostream>
#include <sstream>
#include <thread>
#include <functional>

namespace logbook {

class Internal {
public:
	static unsigned int getThreadNo(std::thread::id threadId);
	static std::pair<Id*, std::reference_wrapper<std::ostream>> pushCurrent(Id id, bool** enabled);
	static void popCurrent();
	static void popCurrent(std::stringstream& sstream, Id& id);

private:
	Internal() = delete;
	~Internal() = delete;
};

} /* namespace logbook */

#endif /* LOGBOOK_APPENDER_INTERNAL_H_ */
