#pragma once

#include <string>

#include "InputHandler.hpp"
#include "Camera.hpp"


class TranslateViewHandler : public InputHandler {
public:
	TranslateViewHandler(Camera& camera);

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	Camera& camera;
};
