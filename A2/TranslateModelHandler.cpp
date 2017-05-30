#define GLM_SWIZZLE // enables vec3.xy()

#include "TranslateModelHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


TranslateModelHandler::TranslateModelHandler(
	ResettableMat4& translationMatrix,
	const ResettableMat4& rotationMatrix
)
: translationMatrix(translationMatrix),
	rotationMatrix(rotationMatrix)
{}

std::string TranslateModelHandler::getName() const {
	return "Translate Model";
}


const double SENSITIVITY = 0.01;

void TranslateModelHandler::translate(double prevPos, double curPos, int index) {
	glm::vec4 worldUnitVector(0, 0, 0, 0);
	worldUnitVector[index] = 1;
	glm::vec4 modelUnitVector = rotationMatrix.matrix * worldUnitVector;

	float delta = (curPos - prevPos) * SENSITIVITY;
	glm::vec3 deltaVector = delta * modelUnitVector.xyz();

	translationMatrix.matrix = matutils::translationMatrix(deltaVector) * translationMatrix.matrix;
}


// translate along x axis
void TranslateModelHandler::onLeftMouseDrag(double prevPos, double curPos) {
	translate(prevPos, curPos, 0);
}

// translate along y axis
void TranslateModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	translate(prevPos, curPos, 1);
}

// translate along z axis
void TranslateModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	translate(prevPos, curPos, 2);
}
