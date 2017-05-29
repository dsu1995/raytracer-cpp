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
	double delta = curPos - prevPos;

	glm::mat4 scaleMatrix = matutils::scaleMatrix(glm::vec3(1 + delta * SENTIVITY, 1, 1));

	cubeModelMatrix.matrix = scaleMatrix * cubeModelMatrix.matrix;
}

// translate along y axis
void ScaleModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 scaleMatrix = matutils::scaleMatrix(glm::vec3(1, 1 + delta * SENTIVITY, 1));

	cubeModelMatrix.matrix = scaleMatrix * cubeModelMatrix.matrix;
}

// translate along z axis
void ScaleModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 scaleMatrix = matutils::scaleMatrix(glm::vec3(1, 1, 1 + delta * SENTIVITY));

	cubeModelMatrix.matrix = scaleMatrix * cubeModelMatrix.matrix;
}
