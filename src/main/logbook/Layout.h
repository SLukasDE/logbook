/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LAYOUT_H_
#define LOGBOOK_LAYOUT_H_

#include <logbook/Location.h>
#include <string>

namespace logbook {
inline namespace v0_4 {

class Layout {
public:
	Layout() = default;
	virtual ~Layout() = default;

	virtual std::string toString(const Location& location) const = 0;
};

} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_LAYOUT_H_ */
