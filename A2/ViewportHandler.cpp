#define GLM_SWIZZLE // enables vec3.xy()

#include "ViewportHandler.hpp"

#include <glm/glm.hpp>

#include "matutils.hpp"

#include <cmath>
#include <algorithm>

#include <iostream>


ViewportHandler::ViewportHandler(Viewport& viewport)
: viewport(viewport),
	mouseDown(false)
{}

void ViewportHandler::reset() {
	mouseDown = false;
}

std::string ViewportHandler::getName() const {
	return "Viewport";
}

void ViewportHandler::onLeftMousePress(double xPos, double yPos) {
	xPos = std::max(-1., std::min(xPos, 1.));
	yPos = std::max(-1., std::min(yPos, 1.));

	mouseDown = true;

	mouseDownYPos = viewport.top = viewport.bottom = yPos;
	mouseDownXPos = viewport.left = viewport.right = xPos;
}

void ViewportHandler::onLeftMouseDragXY(double xPos, double yPos) {
	if (!mouseDown) return;

	xPos = std::max(-1., std::min(xPos, 1.));
	yPos = std::max(-1., std::min(yPos, 1.));

	if (xPos > mouseDownXPos) {
		viewport.right = xPos;
		viewport.left = mouseDownXPos;
	}
	else {
		viewport.right = mouseDownXPos;
		viewport.left = xPos;
	}

	if (yPos > mouseDownYPos) {
		viewport.top = yPos;
		viewport.bottom = mouseDownYPos;
	}
	else {
		viewport.top = mouseDownYPos;
		viewport.bottom = yPos;
	}
}

void ViewportHandler::onLeftMouseRelease(double xPos, double yPos) {
	mouseDown = false;
}


