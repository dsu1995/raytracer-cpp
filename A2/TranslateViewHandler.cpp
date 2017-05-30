#include "TranslateViewHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"


TranslateViewHandler::TranslateViewHandler(Camera& camera)
: camera(camera)
{}

std::string TranslateViewHandler::getName() const {
	return "Translate View";
}


const double SENSITIVITY = 0.005;

// translate along x axis
void TranslateViewHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = (curPos - prevPos) * SENSITIVITY;

	camera.translate(glm::vec3(delta, 0, 0));
}

// translate along y axis
void TranslateViewHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = (curPos - prevPos) * SENSITIVITY;

	camera.translate(glm::vec3(0, delta, 0));
}

// translate along z axis
void TranslateViewHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = (curPos - prevPos) * SENSITIVITY;

	camera.translate(glm::vec3(0, 0, delta));
}
