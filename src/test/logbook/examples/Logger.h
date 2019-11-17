#ifndef LOGBOOK_EXAMPLES_LOGGER_H_
#define LOGBOOK_EXAMPLES_LOGGER_H_

#include <logbook/Logger.h>

namespace logbook {
namespace examples {

extern logbook::Logger logger;
void loggerInitialize();
void loggerReplay();

} /* namespace examples */
} /* namespace logbook */

#endif /* LOGBOOK_EXAMPLES_LOGGER_H_ */
