#include "ResettableMat4.hpp"


ResettableMat4::ResettableMat4(const glm::mat4& initial)
: matrix(initial),
	initial(initial)
{}

void ResettableMat4::reset() {
	matrix = initial;
}
