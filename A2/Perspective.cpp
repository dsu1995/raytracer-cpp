#include <algorithm>

#include "Perspective.hpp"

#include "matutils.hpp"


const float initialFovDegrees = 30;
const float initialNear = 1;
const float initialFar = 1000;

const float MIN_FOV = 5;
const float MAX_FOV = 160;

Perspective::Perspective(float aspectRatio)
: aspectRatio(aspectRatio)
{
	reset();
}

void Perspective::reset() {
	fovDegrees = initialFovDegrees;
	near = initialNear;
	far = initialFar;

	updateMatrix();
}

void Perspective::updateMatrix() {
	matrix = matutils::perspective(
		glm::radians(fovDegrees),
		aspectRatio,
		near,
		far
	);
}

const glm::mat4& Perspective::getMatrix() const {
	return matrix;
}

void Perspective::changeFov(float deltaDegrees) {
	fovDegrees = std::max(MIN_FOV,
		std::min(fovDegrees + deltaDegrees, MAX_FOV)
	);

	updateMatrix();
}

void Perspective::changeNear(float delta) {
	near *= delta;

	updateMatrix();
}

void Perspective::changeFar(float delta) {
	far *= delta;

	updateMatrix();
}

float Perspective::getFovDegrees() const {
	return fovDegrees;
}

float Perspective::getNear() const {
	return near;
}

float Perspective::getFar() const {
	return far;
}

