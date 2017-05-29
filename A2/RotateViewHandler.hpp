#pragma once

#include <string>

#include "InputHandler.hpp"
#include "ResettableMat4.hpp"


class RotateViewHandler : public InputHandler {
public:
	RotateViewHandler(ResettableMat4& view);

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	ResettableMat4& view;
};
