#define GLM_SWIZZLE // enables vec3.xy()

#include "RotateModelHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"

#include <cmath>

#include <iostream>


RotateModelHandler::RotateModelHandler(ResettableMat4& matrix)
: matrix(matrix)
{}

std::string RotateModelHandler::getName() const {
	return "Rotate Model";
}


const double SENSITIVITY = 0.01;

void RotateModelHandler::rotate(double prevPos, double curPos, int index) {
	glm::vec4 worldUnitVector(0, 0, 0, 0);
	worldUnitVector[index] = 1;
	glm::vec4 modelUnitVector = matrix.matrix * worldUnitVector;

	double delta = (curPos - prevPos) * SENSITIVITY;

	matrix.matrix = matutils::rotationAroundAxisMatrix(delta, modelUnitVector.xyz()) * matrix.matrix;
}

// rotate around x axis
void RotateModelHandler::onLeftMouseDrag(double prevPos, double curPos) {
	rotate(prevPos, curPos, 0);
}

// rotate around y axis
void RotateModelHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	rotate(prevPos, curPos, 1);
}

// rotate around z axis
void RotateModelHandler::onRightMouseDrag(double prevPos, double curPos) {
	rotate(prevPos, curPos, 2);
}
