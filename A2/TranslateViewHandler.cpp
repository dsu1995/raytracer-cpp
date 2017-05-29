#include "TranslateViewHandler.hpp"

#include <glm/glm.hpp>


TranslateViewHandler::TranslateViewHandler(ResettableMat4& view)
: view(view)
{}

TranslateViewHandler::~TranslateViewHandler() {}

std::string TranslateViewHandler::getName() const {
	return "Translate View";
}


const double SENTIVITY = 0.5;

// translate along x axis
void TranslateViewHandler::onLeftMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.matrix = matutils::translationMatrix(glm::vec3(delta * SENTIVITY, 0, 0)) * view.matrix;
}

// translate along y axis
void TranslateViewHandler::onMiddleMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.matrix = matutils::translationMatrix(glm::vec3(0, delta * SENTIVITY, 0)) * view.matrix;
}

// translate along z axis
void TranslateViewHandler::onRightMouseDrag(double prevPos, double curPos) {
	double delta = curPos - prevPos;

	view.matrix = matutils::translationMatrix(glm::vec3(0, 0, delta * SENTIVITY)) * view.matrix;
}
