/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_LOCATION_H_
#define LOGBOOK_LOCATION_H_

#include <logbook/Level.h>
#include <thread>
#include <cstring>
#include <ctime>

namespace logbook {
inline namespace v0_4 {

struct Location {
	Location() = default;

	Location(Level aLevel, const void* aObject, const char* aTypeName, const char* aFunction, const char* aFile, std::size_t aLine, std::thread::id aThreadId)
	: level(aLevel),
	  object(aObject),
	  typeName(aTypeName),
	  function(aFunction),
	  file(aFile),
	  line(aLine),
	  threadId(aThreadId)
	{ }

	inline static int stringCmp(const char* s1, const char* s2) {
		if(s1 == nullptr) {
			return s2 == nullptr ? 0 : -1;
		}
		if(s2 == nullptr) {
			return 1;
		}
		return std::strcmp(s1, s2);
	}

	inline static int stringEqual(const char* s1, const char* s2) {
		return stringCmp(s1, s2) == 0;
	}

	inline bool operator==(const Location& location) const {
		return timestamp == location.timestamp &&
				level == location.level &&
				object == location.object &&
				stringEqual(typeName, location.typeName) &&
				stringEqual(function, location.function) &&
				stringEqual(file, location.file) &&
				line == location.line &&
				threadId == location.threadId;
	}

	inline bool operator!=(const Location& location) const {
		return !(*this == location);
	}

	std::time_t timestamp = std::time(nullptr);
	Level level = Level::silent;
	const void* object = nullptr;
	const char* typeName = nullptr;
	const char* function = nullptr;
	const char* file = nullptr;
	std::size_t line = 0;
	std::thread::id threadId = std::this_thread::get_id();

	// this is just an additional payload and has no effect to constructor, comperator etc.
	bool enabled = true;
};

} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_LOCATION_H_ */
