/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_ID_H_
#define LOGBOOK_ID_H_

#include <logbook/Level.h>
#include <thread>
#include <cstring>
#include <ctime>

namespace logbook {

struct Id {
	Id() = default;

	Id(Level level, void* object, const char* typeName, const char* function, const char* file, std::size_t line, std::thread::id threadId)
	: level(level),
	  object(object),
	  typeName(typeName),
	  function(function),
	  file(file),
	  line(line),
	  threadId(threadId)
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

	inline bool operator==(const Id& id) const {
		return timestamp == id.timestamp &&
				level == id.level &&
				object == id.object &&
				stringEqual(typeName, id.typeName) &&
				stringEqual(function, id.function) &&
				stringEqual(file, id.file) &&
				line == id.line &&
				threadId == id.threadId;
	}

	inline bool operator!=(const Id& id) const {
		return !(*this == id);
	}

	std::time_t timestamp = std::time(nullptr);
	Level level = Level::SILENT;
	void* object = nullptr;
	const char* typeName = nullptr;
	const char* function = nullptr;
	const char* file = nullptr;
	std::size_t line = 0;
	std::thread::id threadId = std::this_thread::get_id();

	// this is just an additional payload and has no effect to constructor, comperator etc.
	bool enabled = true;
};

} /* namespace logbook */

#endif /* LOGBOOK_ID_H_ */
