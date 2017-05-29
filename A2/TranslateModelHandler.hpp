#pragma once

#include <string>

#include "InputHandler.hpp"
#include "ResettableMat4.hpp"


class TranslateModelHandler : public InputHandler {
public:
	TranslateModelHandler(ResettableMat4& cubeModelMatrix, ResettableMat4& cubeGnomonModelMatrix);

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	ResettableMat4& cubeModelMatrix;
	ResettableMat4& cubeGnomonModelMatrix;
};
