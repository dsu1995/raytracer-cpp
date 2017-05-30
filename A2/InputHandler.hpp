#pragma once

#include <string>

class InputHandler {
public:
	InputHandler() {}
	virtual ~InputHandler() {}

	virtual std::string getName() const = 0;

	virtual void onLeftMouseDrag(double prevPos, double curPos) {}
	virtual void onMiddleMouseDrag(double prevPos, double curPos) {}
	virtual void onRightMouseDrag(double prevPos, double curPos) {}

	virtual void onLeftMousePress(double xPos, double yPos) {}
	virtual void onLeftMouseDragXY(double xPos, double yPos) {}
	virtual void onLeftMouseRelease(double xPos, double yPos) {}
};
