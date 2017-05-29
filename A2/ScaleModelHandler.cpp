#include "ScaleModelHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


ScaleModelHandler::ScaleModelHandler(ResettableMat4& cubeModelMatrix)
: cubeModelMatrix(cubeModelMatrix),
{}

ScaleModelHandler::~ScaleModelHandler() {}

std::string ScaleModelHandler::getName() const {
	return "Scale Model";
}


const double SENTIVITY = 0.05;

// translate along x axis
void ScaleModelHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 translationMatrix = matutils::scaleMatrix(glm::vec3(1 + delta * SENTIVITY, 0, 0));

	cubeModelMatrix.matrix = translationMatrix * cubeModelMatrix.matrix;
}

// translate along y axis
void ScaleModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 translationMatrix = matutils::scaleMatrix(glm::vec3(0, 1 + delta * SENTIVITY, 0));

	cubeModelMatrix.matrix = translationMatrix * cubeModelMatrix.matrix;
}

// translate along z axis
void ScaleModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 translationMatrix = matutils::scaleMatrix(glm::vec3(0, 0, 1 + delta * SENTIVITY));

	cubeModelMatrix.matrix = translationMatrix * cubeModelMatrix.matrix;
}
