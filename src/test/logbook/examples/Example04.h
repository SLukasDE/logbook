#ifndef LOGBOOK_EXAMPLES_EXAMPLE04_H_
#define LOGBOOK_EXAMPLES_EXAMPLE04_H_

#include <logbook/examples/Logger.h>

namespace logbook {
namespace examples {

class Example04 {
public:
	Example04() = default;
	~Example04() = default;

	float divide(float a, float b);
};

void example04();

} /* namespace examples */
} /* namespace logbook */

#endif /* LOGBOOK_EXAMPLES_EXAMPLE04_H_ */
