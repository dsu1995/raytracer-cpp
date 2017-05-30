#pragma once

#include <string>

#include "InputHandler.hpp"
#include "ResettableMat4.hpp"


class RotateModelHandler : public InputHandler {
public:
	RotateModelHandler(ResettableMat4& matrix);

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	ResettableMat4& matrix;

	void rotate(double prevPos, double curPos, int index);
};
