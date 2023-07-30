#ifndef LOGBOOK_EXAMPLES_LOGGER_H_
#define LOGBOOK_EXAMPLES_LOGGER_H_

#include <logbook/Logger.h>

inline namespace v0_4 {
namespace examples {

extern logbook::Logger logger;
void loggerInitialize();
void loggerReplay();

} /* namespace examples */
} /* inline namespace v0_4 */

#endif /* LOGBOOK_EXAMPLES_LOGGER_H_ */
