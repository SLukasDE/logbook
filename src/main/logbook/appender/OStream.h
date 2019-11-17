/*
Copyright (c) 2019, Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_APPENDER_OSTREAM_H_
#define LOGBOOK_APPENDER_OSTREAM_H_

#include <logbook/Appender.h>
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
	void flushNewLine(const Id& id, bool enabled) override;
	void write(const Id& id, bool enabled, const char* ptr, std::size_t size) override;

private:
	std::ostream& getOStream(Level level);

	bool isFirstCharacterInLine = true;
	std::ostream& oStreamTrace;
	std::ostream& oStreamDebug;
	std::ostream& oStreamInfo;
	std::ostream& oStreamWarn;
	std::ostream& oStreamError;
};

} /* namespace appender */
} /* namespace logbook */

#endif /* LOGBOOK_APPENDER_OSTREAM_H_ */
