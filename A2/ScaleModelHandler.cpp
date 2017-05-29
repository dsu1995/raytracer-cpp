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

	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);
	glm::mat4 translateToOrigin = matutils::translationMatrix(-translation);
	glm::mat4 translateToPosition = matutils::translationMatrix(translation);

	glm::mat4 scaleMatrix =
		translateToPosition *
		matutils::scaleMatrix(glm::vec3(1 + delta * SENTIVITY, 1, 1)) *
		translateToOrigin;

	cubeModelMatrix.matrix = scaleMatrix * cubeModelMatrix.matrix;
}

// translate along y axis
void ScaleModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);
	glm::mat4 translateToOrigin = matutils::translationMatrix(-translation);
	glm::mat4 translateToPosition = matutils::translationMatrix(translation);

	glm::mat4 scaleMatrix =
		translateToPosition *
		matutils::scaleMatrix(glm::vec3(1, 1 + delta * SENTIVITY, 1)) *
		translateToOrigin;

	cubeModelMatrix.matrix = scaleMatrix * cubeModelMatrix.matrix;
}

// translate along z axis
void ScaleModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);
	glm::mat4 translateToOrigin = matutils::translationMatrix(-translation);
	glm::mat4 translateToPosition = matutils::translationMatrix(translation);

	glm::mat4 scaleMatrix =
		translateToPosition *
		matutils::scaleMatrix(glm::vec3(1, 1, 1 + delta * SENTIVITY)) *
		translateToOrigin;

	cubeModelMatrix.matrix = scaleMatrix * cubeModelMatrix.matrix;
}
