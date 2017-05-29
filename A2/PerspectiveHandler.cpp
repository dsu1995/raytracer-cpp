#include "PerspectiveHandler.hpp"

#include <glm/glm.hpp>


PerspectiveHandler::PerspectiveHandler(Perspective& perspective)
: perspective(perspective)
{}

std::string PerspectiveHandler::getName() const {
	return "Perspective";
}


// change FOV
void PerspectiveHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	perspective.changeFov(delta);
}

const double SENTIVITY = 0.5;

// move near plane
void PerspectiveHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	perspective.changeNear(delta * SENTIVITY);
}

// move far plane
void PerspectiveHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	perspective.changeFar(delta * SENTIVITY);
}
