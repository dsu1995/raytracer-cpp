#include "TranslateModelHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


TranslateModelHandler::TranslateModelHandler(ResettableMat4& cubeModelMatrix, ResettableMat4& cubeGnomonModelMatrix)
: cubeModelMatrix(cubeModelMatrix),
	cubeGnomonModelMatrix(cubeGnomonModelMatrix)
{}

TranslateModelHandler::~TranslateModelHandler() {}

std::string TranslateModelHandler::getName() const {
	return "Translate Model";
}


const double SENTIVITY = 0.5;

// translate along x axis
void TranslateModelHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 translationMatrix = matutils::translationMatrix(glm::vec3(delta * SENTIVITY, 0, 0));

	cubeModelMatrix.matrix = translationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = translationMatrix * cubeGnomonModelMatrix.matrix;
}

// translate along y axis
void TranslateModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 translationMatrix = matutils::translationMatrix(glm::vec3(0, delta * SENTIVITY, 0));

	cubeModelMatrix.matrix = translationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = translationMatrix * cubeGnomonModelMatrix.matrix;
}

// translate along z axis
void TranslateModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 translationMatrix = matutils::translationMatrix(glm::vec3(0, 0, delta * SENTIVITY));

	cubeModelMatrix.matrix = translationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = translationMatrix * cubeGnomonModelMatrix.matrix;
}
