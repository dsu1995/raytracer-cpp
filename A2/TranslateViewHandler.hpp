#pragma once

#include <string>

#include "InputHandler.hpp"
#include "View.hpp"


class TranslateViewHandler : public InputHandler {
public:
	TranslateViewHandler(View& view);
	virtual ~TranslateViewHandler();

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	View& view;
};
