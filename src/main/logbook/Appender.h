/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_APPENDER_H_
#define LOGBOOK_APPENDER_H_

#include <logbook/Logger.h>
#include <logbook/Id.h>
#include <logbook/Layout.h>
#include <cstddef>

namespace logbook {

class Logger;

class Appender {
friend class Logger;
public:
	enum class RecordLevel {
		ALL, SELECTED, OFF
	};

	Appender() = default;
	virtual ~Appender() = default;

	inline Layout& getLayout() {
		return layout;
	}

	/* method is NOT thread-safe */
	inline RecordLevel getRecordLevel() const {
		return recordLevel;
	}

	/* method is NOT thread-safe */
	inline void setRecordLevel(RecordLevel recordLevel = RecordLevel::SELECTED) {
		this->recordLevel = recordLevel;
	}

protected:
	virtual void flushNewLine(const Id& id, bool enabled) = 0;
	virtual void write(const Id& id, bool enabled, const char* ptr, std::size_t size) = 0;

private:
	Layout layout;
	RecordLevel recordLevel = RecordLevel::SELECTED;
};

} /* namespace logbook */

#endif /* LOGBOOK_APPENDER_H_ */
