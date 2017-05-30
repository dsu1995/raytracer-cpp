#include "RotateViewHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


RotateViewHandler::RotateViewHandler(Camera& camera)
: camera(camera)
{}

std::string RotateViewHandler::getName() const {
	return "Rotate View";
}


const double SENSITIVITY = 0.0005;

// rotate around x axis
void RotateViewHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = (curPos - prevPos) * SENSITIVITY;

	camera.lookUpDown(delta);
}

// rotate around y axis
void RotateViewHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = (curPos - prevPos) * SENSITIVITY;

	camera.lookSideToSide(delta);
}

// rotate around z axis
void RotateViewHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = (curPos - prevPos) * SENSITIVITY;

	camera.tiltView(delta);
}
