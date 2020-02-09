/*
Copyright (c) 2019, 2020, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_APPENDER_OSTREAM_H_
#define LOGBOOK_APPENDER_OSTREAM_H_

#include <logbook/Appender.h>
#include <logbook/Location.h>
#include <logbook/Level.h>
#include <string>
#include <ostream>

namespace logbook {
namespace appender {

class OStream : public Appender {
public:
	OStream(std::ostream& oStreamTrace,
	std::ostream& oStreamDebug,
	std::ostream& oStreamInfo,
	std::ostream& oStreamWarn,
	std::ostream& oStreamError);

protected:
	void flush() override;
	void write(const Location& location, const char* ptr, std::size_t size) override;

private:
	bool isFirstCharacterInLine = true;
	Location lastLocation;
	std::ostream& oStreamTrace;
	std::ostream& oStreamDebug;
	std::ostream& oStreamInfo;
	std::ostream& oStreamWarn;
	std::ostream& oStreamError;

	std::ostream& getOStream(Level level);
};

} /* namespace appender */
} /* namespace logbook */

#endif /* LOGBOOK_APPENDER_OSTREAM_H_ */
