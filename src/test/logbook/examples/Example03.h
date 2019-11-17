#ifndef LOGBOOK_EXAMPLES_EXAMPLE03_H_
#define LOGBOOK_EXAMPLES_EXAMPLE03_H_

#include <logbook/Logger.h>

namespace logbook {
namespace examples {

class Example03 {
public:
	Example03() = default;
	~Example03() = default;

	float divide(float a, float b);

private:
	static logbook::Logger logger;
};

void example03();

} /* namespace examples */
} /* namespace logbook */

#endif /* LOGBOOK_EXAMPLES_EXAMPLE03_H_ */
