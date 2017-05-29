#include "View.hpp"

#include "matutils.hpp"


const glm::mat4 initialMatrix = matutils::lookAt(
	glm::vec3(
		0.0f,
		0.5f * M_SQRT1_2,
		0.5f * M_SQRT1_2
	),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f)
);


View::View()
{
	reset();
}

View::~View() {}

void View::reset() {
	matrix = initialMatrix;
}

void View::rotateX(float radians) {
	matrix = matutils::rotationMatrixX(radians) * matrix;
}

void View::rotateY(float radians) {
	matrix = matutils::rotationMatrixY(radians) * matrix;
}

void View::rotateZ(float radians) {
	matrix = matutils::rotationMatrixZ(radians) * matrix;
}

void View::translate(const glm::vec3& delta) {
	matrix = matutils::translationMatrix(delta) * matrix;
}

const glm::mat4& View::getMatrix() const {
	return matrix;
}
