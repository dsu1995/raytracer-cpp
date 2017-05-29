#include "PerspectiveHandler.hpp"

#include <glm/glm.hpp>


PerspectiveHandler::PerspectiveHandler(Perspective& perspective)
: perspective(perspective)
{}

PerspectiveHandler::~PerspectiveHandler() {}

std::string PerspectiveHandler::getName() const {
	return "Perspective";
}


// change FOV
void PerspectiveHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	perspective.changeFOV(delta);
}

const double SENTIVITY = 0.5;

// move near plane
void PerspectiveHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.changeNear(delta * SENTIVITY);
}

// move far plane
void PerspectiveHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.changeFar(delta * SENTIVITY);
}
