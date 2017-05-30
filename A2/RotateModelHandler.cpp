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

	float x = modelYUnitVector.x;
	float y = modelYUnitVector.y;
	float z = modelYUnitVector.z;

	float phi = atan2(z, x);
	float psi = 3 * M_PI / 2 + atan2(y, sqrt(x * x + z * z));

	glm::vec3 translation = matutils::getTranslation(cubeModelMatrix.matrix);

	double delta = (curPos - prevPos) * SENSITIVITY;
	glm::mat4 rotationMatrix =
		matutils::translationMatrix(translation) *
		matutils::rotationMatrixY(phi) *
		matutils::rotationMatrixZ(psi) *
		matutils::rotationMatrixY(delta) *
		matutils::rotationMatrixZ(-psi) *
		matutils::rotationMatrixY(-phi) *
		matutils::translationMatrix(-translation);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}

// rotate around z axis
void RotateModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	glm::mat4 rotationMatrix = matutils::rotationMatrixZ(delta * SENSITIVITY);

	cubeModelMatrix.matrix = rotationMatrix * cubeModelMatrix.matrix;
	cubeGnomonModelMatrix.matrix = rotationMatrix * cubeGnomonModelMatrix.matrix;
}
