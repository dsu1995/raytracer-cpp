#pragma once

#include <string>

#include "InputHandler.hpp"


class RotateViewHandler : public InputHandler {
public:
	RotateViewHandler(View& view);
	virtual ~RotateViewHandler();

	virtual std::string getName() const = 0;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	View& view;
};
