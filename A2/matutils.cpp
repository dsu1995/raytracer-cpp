#define GLM_SWIZZLE // enables vec3.xy()

#include "matutils.hpp"

#include <cmath>

#include <iostream>

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

	// source: https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	glm::mat4 rotationAroundAxisMatrix(float radians, const glm::vec3& axis) {
		glm::vec3 normalizedAxis = glm::normalize(axis);

		float x = normalizedAxis.x;
		float y = normalizedAxis.y;
		float z = normalizedAxis.z;

		float c = cos(radians);
		float s = sin(radians);

		glm::mat4 R;
		R[0][0] = c + x*x*(1-c);
		R[1][0] = x*y*(1-c) - z*s;
		R[2][0] = x*z*(1-c) + y*s;
		R[0][1] = y*x*(1-c) + z*s;
		R[1][1] = c + y*y*(1-c);
		R[2][1] = y*z*(1-c) - x*s;
		R[0][2] = z*x*(1-c) - y*s;
		R[1][2] = z*y*(1-c) + x*s;
		R[2][2] = c + z*z*(1-c);
		return R;
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

	glm::mat4 perspective(
		float fov,
		float aspectRatio,
		float near,
		float far
	) {
		float cotTheta_2 = 1 / tan(fov / 2);

		glm::mat4 result;
		result[0][0] = cotTheta_2 / aspectRatio;
		result[1][1] = cotTheta_2;
		result[2][2] = (far + near) / (far - near);
		result[3][2] = -2 * far * near / (far - near);
		result[2][3] = 1;
		result[3][3] = 0;

		return result;
	}

	glm::vec2 homogenize(glm::vec4 v) {
		return (1 / v.z) * v.xy();
	}

	glm::vec3 getTranslation(glm::mat4 mat) {
		return (mat * glm::vec4(0, 0, 0, 1)).xyz();
	}
}
