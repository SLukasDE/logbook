#include <logbook/examples/Logger.h>
#include <logbook/appender/OStream.h>
#include <logbook/appender/MemBuffer.h>
#include <logbook/Id.h>

#include <vector>
#include <tuple>
#include <iostream>
#include <string>

inline namespace v0_4 {
namespace examples {

namespace {
logbook::appender::OStream appenderOstream(std::cout, std::cout, std::cout, std::cout, std::cout);

/* MemBuffer appender just writes output to a buffer of a fixed number of lines.
 * If number of columns is specified as well the whole memory is allocated at initialization time.
 */
logbook::appender::MemBuffer appenderMemBuffer(100);
//logbook::appender::MemBuffer appenderMemBuffer(100, 200);
}

logbook::Logger logger("logbook::examples");

void loggerInitialize() {
	appenderOstream.setRecordLevel();
	appenderOstream.getLayout().setShowFile(true);
	appenderOstream.getLayout().setShowLineNo(true);
	logbook::Logger::addAppender(appenderOstream);

	/* Setting record level to ALL to an appender makes him catching all log messages, even if global logging level is set to SILENT */
	appenderMemBuffer.setRecordLevel(logbook::Appender::RecordLevel::ALL);
	appenderMemBuffer.getLayout().setShowFile(true);
	appenderMemBuffer.getLayout().setShowLineNo(true);
	logbook::Logger::addAppender(appenderMemBuffer);

	/* setting default */
	logbook::Logger::setLevel(logbook::Level::WARN, "*");

	//esl::logging::Logger::setLevel(esl::logging::Level::INFO, "AAA::BBB::*");
	//esl::logging::Logger::setLevel(esl::logging::Level::DEBUG, "AAA::BBB::CCC::*");
}

void loggerReplay() {
	appenderMemBuffer.getLayout().setShowTypeName(false);
	std::vector<std::tuple<logbook::Id, std::string>> buffer = appenderMemBuffer.getBuffer();
	std::cout << "Replay: " << buffer.size() << " elements\n";
	for(const auto& line : buffer) {
		std::cout << appenderMemBuffer.getLayout().makePrefix(std::get<0>(line)) << std::get<1>(line) << "\n";
	}
}


} /* namespace examples */
} /* inline namespace v0_4 */
