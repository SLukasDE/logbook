/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LEVEL_H_
#define LOGBOOK_LEVEL_H_

namespace logbook {

enum class Level {
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	SILENT
};

} /* namespace logbook */

#endif /* LOGBOOK_LEVEL_H_ */
