#pragma once

#include <string>

#include "InputHandler.hpp"
#include "ResettableMat4.hpp"


class TranslateModelHandler : public InputHandler {
public:
	TranslateModelHandler(
		ResettableMat4& translationMatrix,
		const ResettableMat4& rotationMatrix
	);

	virtual std::string getName() const;

	virtual void onLeftMouseDrag(double prevPos, double curPos);
	virtual void onMiddleMouseDrag(double prevPos, double curPos);
	virtual void onRightMouseDrag(double prevPos, double curPos);

private:
	ResettableMat4& translationMatrix;
	const ResettableMat4& rotationMatrix;

	void translate(double prevPos, double curPos, int index);
};
