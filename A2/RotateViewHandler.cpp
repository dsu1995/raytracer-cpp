#include "RotateViewHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


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

	view.matrix = matutils::rotationMatrixX(delta * SENTIVITY) * view.matrix;
}

// rotate around y axis
void RotateViewHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.matrix = matutils::rotationMatrixY(delta * SENTIVITY) * view.matrix;
}

// rotate around z axis
void RotateViewHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.matrix = matutils::rotationMatrixZ(delta * SENTIVITY) * view.matrix;
}
