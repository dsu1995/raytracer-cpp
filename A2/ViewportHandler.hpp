#pragma once

#include <string>

#include "InputHandler.hpp"
#include "Viewport.hpp"


class ViewportHandler : public InputHandler {
public:
	ViewportHandler(Viewport& viewport);

	virtual std::string getName() const;

	virtual void onLeftMousePress(double xPos, double yPos);
	virtual void onLeftMouseDragXY(double xPos, double yPos);
	virtual void onLeftMouseRelease(double xPos, double yPos);

	void reset();

private:
	Viewport& viewport;

	bool mouseDown;
	double mouseDownXPos;
	double mouseDownYPos;
};
