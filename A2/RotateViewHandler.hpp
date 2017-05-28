#pragma once

#include <string>

#include "InputHandler.hpp"
#include "View.hpp"


class RotateViewHandler : public InputHandler {
public:
	RotateViewHandler(View& view);
	virtual ~RotateViewHandler();

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	View& view;
};
