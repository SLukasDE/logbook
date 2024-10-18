#ifndef LOGBOOK_EXAMPLES_EXAMPLE04_H_
#define LOGBOOK_EXAMPLES_EXAMPLE04_H_

#include <logbook/examples/Logger.h>

namespace logbook {
inline namespace v0_4 {
namespace examples {

class Example04 {
public:
	Example04() = default;
	~Example04() = default;

	float divide(float a, float b);
};

void example04();

} /* namespace examples */
} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_EXAMPLES_EXAMPLE04_H_ */
