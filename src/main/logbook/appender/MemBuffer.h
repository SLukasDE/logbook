/*
Copyright (c) 2019, Sven Lukas

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
	void flushNewLine(const Location& location, bool enabled) override;
	void write(const Location& location, bool enabled, const char* ptr, std::size_t size) override;

private:
    using LineBuffer = std::vector<char>;

    // this is the static buffer, used if maxColumns>0
    std::vector<LineBuffer> rows;

    // this is the dynamic buffer, used if maxColumns=0
    std::vector<std::string> lines;

    std::vector<Location> locations;

	const std::size_t maxRows;
	const std::size_t maxColumns;

	std::size_t rowProducer = 0;
	std::size_t rowConsumer = 0;
	std::size_t columnsProducer = 0;
};

} /* namespace appender */
} /* namespace logbook */

#endif /* LOGBOOK_APPENDER_MEMBUFFER_H_ */
