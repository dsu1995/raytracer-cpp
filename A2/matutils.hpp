#pragma once

#include <glm/glm.hpp>

namespace matutils {
	glm::mat4 translationMatrix(float dx, float dy, float dz);

	glm::mat4 scaleMatrix(float sx, float sy, float sz);

	glm::mat4 rotationMatrixZ(float radians);
	glm::mat4 rotationMatrixX(float radians);
	glm::mat4 rotationMatrixY(float radians);

	glm::mat4 lookAt(
		const glm::vec3& from,
		const glm::vec3& at,
		const glm::vec3& up
	);
}
