#include <logbook/examples/Logger.h>
#include <logbook/appender/OStream.h>
#include <logbook/appender/MemBuffer.h>
//#include <logbook/Id.h>
#include <logbook/layout/Default.h>

#include <vector>
#include <tuple>
#include <iostream>
#include <string>

namespace logbook {
inline namespace v0_4 {
namespace examples {

namespace {
logbook::appender::OStream appenderOstream(std::cout, std::cout, std::cout, std::cout, std::cout);
logbook::layout::Default defaultOstreamLayout;

/* MemBuffer appender just writes output to a buffer of a fixed number of lines.
 * If number of columns is specified as well the whole memory is allocated at initialization time.
 */
logbook::appender::MemBuffer appenderMemBuffer(100);
logbook::layout::Default defaultMemBufferLayout;
//logbook::appender::MemBuffer appenderMemBuffer(100, 200);
}

logbook::Logger logger("logbook::examples");

void loggerInitialize() {
	defaultOstreamLayout.setShowFile(true);
	defaultOstreamLayout.setShowLineNo(true);
	appenderOstream.setRecordLevel();
	appenderOstream.setLayout(&defaultOstreamLayout);
	//logbook::addAppender(appenderOstream);

	/* Setting record level to ALL to an appender makes him catching all log messages, even if global logging level is set to SILENT */
	defaultMemBufferLayout.setShowFile(true);
	defaultMemBufferLayout.setShowLineNo(true);
	appenderMemBuffer.setRecordLevel(logbook::Appender::RecordLevel::ALL);
	appenderMemBuffer.setLayout(&defaultMemBufferLayout);
	//logbook::addAppender(appenderMemBuffer);

	/* setting default */
	logbook::setLevel(logbook::Level::warn, "*");

	//esl::logging::Logger::setLevel(Level::info, "AAA::BBB::*");
	//esl::logging::Logger::setLevel(Level::debug, "AAA::BBB::CCC::*");
}

void loggerReplay() {
	if(appenderMemBuffer.getLayout() == &defaultMemBufferLayout) {
		defaultMemBufferLayout.setShowTypeName(false);
	}
	std::vector<std::tuple<logbook::Location, std::string>> buffer = appenderMemBuffer.getBuffer();
	std::cout << "Replay: " << buffer.size() << " elements\n";
	for(const auto& line : buffer) {
		if(appenderMemBuffer.getLayout()) {
			std::cout << appenderMemBuffer.getLayout()->toString(std::get<0>(line)) << std::get<1>(line) << "\n";
		}
		else {
			std::cout << std::get<1>(line) << "\n";
		}
	}
}


} /* namespace examples */
} /* inline namespace v0_4 */
} /* namespace logbook */
