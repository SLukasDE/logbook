/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LEVEL_H_
#define LOGBOOK_LEVEL_H_

namespace logbook {
inline namespace v0_4 {

enum class Level {
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	SILENT
};

} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_LEVEL_H_ */
