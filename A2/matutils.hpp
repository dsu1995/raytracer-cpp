#pragma once

#include <glm/glm.hpp>

namespace matutils {
	glm::mat4 translationMatrix(const glm::vec3& delta);

	glm::mat4 scaleMatrix(const glm::vec3& scale);

	glm::mat4 rotationMatrixZ(float radians);
	glm::mat4 rotationMatrixX(float radians);
	glm::mat4 rotationMatrixY(float radians);

	glm::mat4 lookAt(
		const glm::vec3& from,
		const glm::vec3& at,
		const glm::vec3& up
	);

	glm::mat4 perspective(
		float fov,
		float aspectRatio,
		float near,
		float far
	);

	glm::vec2 homogenize(glm::vec4 v);

	glm::vec3 getTranslation(glm::mat4 mat);
}
