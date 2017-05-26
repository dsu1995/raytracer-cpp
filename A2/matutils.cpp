#include "matutils.hpp"

#include <cmath>

namespace matutils {
	glm::mat4 translationMatrix(const glm::vec3& delta) {
		glm::mat4 m;
		m[3] = glm::vec4(delta, 1);
		return m;
	}

	glm::mat4 scaleMatrix(const glm::vec3& s) {
		glm::mat4 m;
		m[0][0] = s.x;
		m[1][1] = s.y;
		m[2][2] = s.z;
		return m;
	}

	glm::mat4 rotationMatrixZ(float radians) {
		float c = cos(radians);
		float s = sin(radians);

		glm::mat4 m;
		m[0][0] = c;
		m[1][0] = -s;
		m[0][1] = s;
		m[1][1] = c;
		return m;
	}

	glm::mat4 rotationMatrixX(float radians) {
		float c = cos(radians);
		float s = sin(radians);

		glm::mat4 m;
		m[1][1] = c;
		m[2][1] = -s;
		m[1][2] = s;
		m[2][2] = c;
		return m;
	}

	glm::mat4 rotationMatrixY(float radians) {
		float c = cos(radians);
		float s = sin(radians);

		glm::mat4 m;
		m[0][0] = c;
		m[2][0] = s;
		m[0][2] = -s;
		m[2][2] = c;
		return m;
	}

	glm::mat4 lookAt(
		const glm::vec3& from,
		const glm::vec3& at,
		const glm::vec3& up
	) {
		glm::vec3 v_z = glm::normalize(at - from);
		glm::vec3 v_x = glm::normalize(glm::cross(up, v_z));
		glm::vec3 v_y = glm::cross(v_z, v_x);

		glm::mat4 R;
		R[0][0] = v_x.x; R[1][0] = v_x.y; R[2][0] = v_x.z;
		R[0][1] = v_y.x; R[1][1] = v_y.y; R[2][1] = v_y.z;
		R[0][2] = v_z.x; R[1][2] = v_z.y; R[2][2] = v_z.z;

		return R * translationMatrix(-from);
	}
}