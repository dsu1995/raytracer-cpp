#include "ScaleModelHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


ScaleModelHandler::ScaleModelHandler(ResettableMat4& cubeModelMatrix)
: cubeModelMatrix(cubeModelMatrix)
{}

std::string ScaleModelHandler::getName() const {
	return "Scale Model";
}


const double SENTIVITY = 0.001;

// translate along x axis
void ScaleModelHandler::onLeftMouseDrag(double prevPos, double curPos) {
	scale(prevPos, curPos, 0);
}

// translate along y axis
void ScaleModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	scale(prevPos, curPos, 1);
}

// translate along z axis
void ScaleModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	scale(prevPos, curPos, 2);
}

void ScaleModelHandler::scale(double prevPos, double curPos, int index) {
	double scaleFactor = 1 + (curPos - prevPos) * SENTIVITY;
	glm::vec3 scaleVector(1, 1, 1);
	scaleVector[index] = scaleFactor;
	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);

	glm::mat4 scaleMatrix =
		matutils::translationMatrix(translation) *
		matutils::scaleMatrix(scaleVector) *
		matutils::translationMatrix(-translation);

	cubeModelMatrix.matrix = scaleMatrix * cubeModelMatrix.matrix;
}
