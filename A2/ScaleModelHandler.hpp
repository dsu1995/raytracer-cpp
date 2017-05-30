#pragma once

#include <string>

#include "InputHandler.hpp"
#include "ResettableMat4.hpp"


class ScaleModelHandler : public InputHandler {
public:
	ScaleModelHandler(ResettableMat4& cubeModelMatrix);

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	ResettableMat4& cubeModelMatrix;

	void scale(double prevPos, double curPos, int index);
};
