#pragma once

#include <glm/glm.hpp>


class Perspective {
public:
	Perspective(float aspectRatio);
	~Perspective();

	void changeFov(float deltaDegrees);
	void changeNear(float delta);
	void changeFar(float delta);

	const glm::mat4& getMatrix() const;

	void reset();

private:
	void updateMatrix();

	float aspectRatio;
	float fovDegrees;
	float near;
	float far;

	glm::mat4 matrix;
};
