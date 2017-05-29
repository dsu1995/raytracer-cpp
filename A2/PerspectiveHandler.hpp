#pragma once

#include <string>

#include "InputHandler.hpp"
#include "Perspective.hpp"


class PerspectiveHandler : public InputHandler {
public:
	PerspectiveHandler(Perspective& perspective);

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	Perspective& perspective;
};
