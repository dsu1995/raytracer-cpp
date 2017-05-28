#pragma once

class InputHandler {
public:
	InputHandler();
	virtual ~InputHandler();

	virtual string getName() const = 0;

	virtual void onLeftMouseDrag(double prevPos, double curPos) = 0;
	virtual void onMiddleMouseDrag(double prevPos, double curPos) = 0;
	virtual void onRightMouseDrag(double prevPos, double curPos) = 0;
};
