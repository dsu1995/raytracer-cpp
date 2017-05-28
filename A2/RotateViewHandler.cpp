#include "RotateViewHandler.hpp"

#include <glm/glm.hpp>


RotateViewHandler::RotateViewHandler(View& view)
: view(view)
{}

RotateViewHandler::~RotateViewHandler() {}

std::string RotateViewHandler::getName() const {
	return "Rotate View";
}


const double SENTIVITY = 0.5;

// rotate around x axis
void RotateViewHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.rotateX(delta * SENTIVITY);
}

// rotate around y axis
void RotateViewHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.rotateY(delta * SENTIVITY);
}

// rotate around z axis
void RotateViewHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.rotateZ(delta * SENTIVITY);
}
