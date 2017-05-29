#pragma once

#include <string>

#include "InputHandler.hpp"
#include "ResettableMat4.hpp"


class TranslateViewHandler : public InputHandler {
public:
	TranslateViewHandler(ResettableMat4& view);
	virtual ~TranslateViewHandler();

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	ResettableMat4& view;
};
