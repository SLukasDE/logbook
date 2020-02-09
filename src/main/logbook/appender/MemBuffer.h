/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_APPENDER_MEMBUFFER_H_
#define LOGBOOK_APPENDER_MEMBUFFER_H_

#include <logbook/Appender.h>
#include <logbook/Location.h>
#include <vector>
#include <string>
#include <tuple>

namespace logbook {
namespace appender {

class MemBuffer : public Appender {
public:
	MemBuffer(std::size_t maxRows, std::size_t maxColumns = 0);
	~MemBuffer();

	/* method is (currently) NOT thread-safe */
	std::vector<std::tuple<Location, std::string>> getBuffer() const;

protected:
	void flush() override;
	void write(const Location& location, const char* ptr, std::size_t size) override;

private:
    using LineBuffer = std::vector<char>;
    struct Entry {
    	Entry(std::size_t maxColumns)
    	: lineStaticSize((maxColumns > 0 ? maxColumns+1 : 0), 0)
    	{ }
    	Location location;
    	LineBuffer lineStaticSize;
    	std::string lineDynamicSize;
    };

    std::vector<Entry> entries;

	const std::size_t maxRows;
	const std::size_t maxColumns;

	std::size_t rowProducer = 0;
	std::size_t rowConsumer = 0;
	std::size_t columnsProducer = 0;

	void write(const char* ptr, std::size_t size);
	void newline();
};

} /* namespace appender */
} /* namespace logbook */

#endif /* LOGBOOK_APPENDER_MEMBUFFER_H_ */
