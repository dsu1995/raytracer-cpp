#define GLM_SWIZZLE // enables vec3.xy()

#include "RotateModelHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"

#include <cmath>

#include <iostream>


RotateModelHandler::RotateModelHandler(ResettableMat4& cubeModelMatrix, ResettableMat4& cubeGnomonModelMatrix)
: cubeModelMatrix(cubeModelMatrix),
	cubeGnomonModelMatrix(cubeGnomonModelMatrix)
{}

std::string RotateModelHandler::getName() const {
	return "Rotate Model";
}


const double SENSITIVITY = 0.01;

// rotate around x axis
void RotateModelHandler::onLeftMouseDrag(double prevPos, double curPos) {
	glm::vec4 worldXUnitVector(1, 0, 0, 0);
	glm::vec4 modelXUnitVector = cubeModelMatrix.matrix * worldXUnitVector;

	double delta = (curPos - prevPos) * SENSITIVITY;

	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);

	glm::mat4 rotationMatrix =
		matutils::translationMatrix(translation) *
		matutils::rotationAroundAxisMatrix(delta, modelXUnitVector.xyz()) *
		matutils::translationMatrix(-translation);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}

// rotate around y axis
void RotateModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	glm::vec4 worldYUnitVector(0, 1, 0, 0);
	glm::vec4 modelYUnitVector = cubeModelMatrix.matrix * worldYUnitVector;

	double delta = (curPos - prevPos) * SENSITIVITY;

	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);

	glm::mat4 rotationMatrix =
		matutils::translationMatrix(translation) *
		matutils::rotationAroundAxisMatrix(delta, modelYUnitVector.xyz()) *
		matutils::translationMatrix(-translation);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}

// rotate around z axis
void RotateModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	glm::vec4 worldZUnitVector(0, 0, 1, 0);
	glm::vec4 modelZUnitVector = cubeModelMatrix.matrix * worldZUnitVector;

	double delta = (curPos - prevPos) * SENSITIVITY;

	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);

	glm::mat4 rotationMatrix =
		matutils::translationMatrix(translation) *
		matutils::rotationAroundAxisMatrix(delta, modelZUnitVector.xyz()) *
		matutils::translationMatrix(-translation);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}
