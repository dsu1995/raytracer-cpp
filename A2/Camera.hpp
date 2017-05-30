#pragma once

#include <glm/glm.hpp>


class Camera {
public:
	Camera(
		const glm::vec3& lookFrom,
		const glm::vec3& lookAt,
		const glm::vec3& up
	);

	void lookUpDown(float radians);
	void lookSideToSide(float radians);
	void tiltView(float radians);
	void translate(const glm::vec3& delta);

	const glm::mat4& getMatrix() const;

	void reset();

	const glm::vec3& getLookFrom() const;
	const glm::vec3& getLookAt() const;
	const glm::vec3& getUp() const;

private:
	const glm::vec3 initialLookFrom;
	const glm::vec3 initialLookAt;
	const glm::vec3 initialUp;

	glm::vec3 lookFrom;
	glm::vec3 lookAt;
	glm::vec3 up;

	glm::mat4 matrix;

	void update();
};