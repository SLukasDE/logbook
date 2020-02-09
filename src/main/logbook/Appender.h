/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_APPENDER_H_
#define LOGBOOK_APPENDER_H_

#include <logbook/Layout.h>
#include <cstddef>

namespace logbook {

class Location;

class Appender {
friend class Logbook;
public:
	enum class RecordLevel {
		ALL, SELECTED, OFF
	};

	Appender();
	virtual ~Appender();

	void setLayout(const Layout* aLayout);
	const Layout* getLayout() const;

	/* Both methods are NOT thread-safe */
	RecordLevel getRecordLevel() const;
	void setRecordLevel(RecordLevel aRecordLevel = RecordLevel::SELECTED);

protected:
	virtual void flush() = 0;
	virtual void write(const Location& location, const char* ptr, std::size_t size) = 0;

private:
	const Layout* layout = nullptr;
	RecordLevel recordLevel = RecordLevel::SELECTED;
};

} /* namespace logbook */

#endif /* LOGBOOK_APPENDER_H_ */
