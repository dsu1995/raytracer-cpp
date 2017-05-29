#include "RotateModelHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


RotateModelHandler::RotateModelHandler(ResettableMat4& cubeModelMatrix, ResettableMat4& cubeGnomonModelMatrix)
: cubeModelMatrix(cubeModelMatrix),
	cubeGnomonModelMatrix(cubeGnomonModelMatrix)
{}

std::string RotateModelHandler::getName() const {
	return "Rotate Model";
}


const double SENTIVITY = 0.01;

// rotate around x axis
void RotateModelHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 rotationMatrix = matutils::rotationMatrixX(delta * SENTIVITY);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}

// rotate around y axis
void RotateModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 rotationMatrix = matutils::rotationMatrixY(delta * SENTIVITY);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}

// rotate around z axis
void RotateModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 rotationMatrix = matutils::rotationMatrixZ(delta * SENTIVITY);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}
