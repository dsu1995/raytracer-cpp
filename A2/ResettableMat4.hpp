#pragma once

#include <glm/glm.hpp>


class ResettableMat4 {
public:
	ResettableMat4(const glm::mat4& initial);

	glm::mat4 matrix;

	void reset();

private:
	glm::mat4 initial;
};
