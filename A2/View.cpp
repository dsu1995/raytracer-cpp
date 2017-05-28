#include "View.hpp"

#include "matutils.hpp"

View::View()
: initial{
		matutils::lookAt(
			glm::vec3(
				0.0f,
				0.5f * M_SQRT1_2,
				0.5f * M_SQRT1_2
			),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		)
	}
{
	reset();
}

View::~View() {}

void View::reset() {
	cur = initial;
}

void View::rotateX(float radians) {
	cur = matutils::rotationMatrixX(radians) * cur;
}

void View::rotateY(float radians) {
	cur = matutils::rotationMatrixY(radians) * cur;
}

void View::rotateZ(float radians) {
	cur = matutils::rotationMatrixZ(radians) * cur;
}

void View::translate(const glm::vec3& delta) {
	cur = matutils::translationMatrix(delta) * cur;
}

const glm::mat4& View::getMatrix() const {
	return cur;
}
