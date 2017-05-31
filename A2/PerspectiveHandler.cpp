#include "PerspectiveHandler.hpp"

#include <glm/glm.hpp>


PerspectiveHandler::PerspectiveHandler(Perspective& perspective)
: perspective(perspective)
{}

std::string PerspectiveHandler::getName() const {
	return "Perspective";
}

const double FOV_SENSITIVITY = 0.1;

// change FOV
void PerspectiveHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	perspective.changeFov(delta * FOV_SENSITIVITY);
}

const double NEAR_FAR_SENTIVITY = 0.01;

// move near plane
void PerspectiveHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	perspective.changeNear(delta * NEAR_FAR_SENTIVITY);
}

// move far plane
void PerspectiveHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	perspective.changeFar(delta * NEAR_FAR_SENTIVITY);
}
