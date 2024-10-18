#include <logbook/examples/Example01.h>
#include <logbook/Logger.h>

namespace logbook {
inline namespace v0_4 {
namespace examples {

void example01() {
	/* creating a logger class */
	logbook::Logger logger;

	/* That's it, now you can use it already */
	logger.info << "Hello world!" << std::endl;

	/* Ups, you see nothing.
	 * We have not "initialized" the logging framework.
	 * Let's go to logger example 2 to see how to initialize... */
}

} /* namespace examples */
} /* inline namespace v0_4 */
} /* namespace logbook */
