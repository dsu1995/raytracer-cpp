#define GLM_SWIZZLE // enables vec3.xy()

#include "Camera.hpp"

#include "matutils.hpp"

Camera::Camera(
	const glm::vec3& lookFrom,
	const glm::vec3& lookAt,
	const glm::vec3& up
)
: initialLookFrom(lookFrom),
	initialLookAt(lookAt),
	initialUp(up),
	lookFrom(lookFrom),
	lookAt(lookAt),
	up(up)
{
	update();
}

void Camera::reset() {
	lookFrom = initialLookFrom;
	lookAt = initialLookAt;
	up = initialUp;

	update();
}

void Camera::update() {
	matrix = matutils::lookAt(lookFrom, lookAt, up) * matutils::scaleMatrix(glm::vec3(-1, 1, 1));;
}

void Camera::lookUpDown(float radians) {
	glm::vec3 horizontalAxis = glm::cross(lookAt - lookFrom, up);

	glm::mat4 rotationMatrix = matutils::rotationAroundAxisMatrix(radians, horizontalAxis);
	glm::vec4 rotatedVector = rotationMatrix * glm::vec4(lookAt - lookFrom, 0);
	glm::vec3 rotatedPoint = lookFrom + rotatedVector.xyz();

	lookAt = rotatedPoint;

	update();
}

void Camera::lookSideToSide(float radians) {
	glm::mat4 rotationMatrix = matutils::rotationAroundAxisMatrix(radians, up);
	glm::vec4 rotatedVector = rotationMatrix * glm::vec4(lookAt - lookFrom, 0);
	glm::vec3 rotatedPoint = lookFrom + rotatedVector.xyz();

	lookAt = rotatedPoint;

	update();
}

void Camera::tiltView(float radians) {
	glm::vec3 rotationAxis = lookAt - lookFrom;

	up = (matutils::rotationAroundAxisMatrix(radians, rotationAxis) * glm::vec4(up, 0)).xyz();

	update();
}

void Camera::translate(const glm::vec3& delta) {
	glm::vec3 xUnit = glm::normalize(glm::cross(lookAt - lookFrom, up));
	glm::vec3 yUnit = glm::normalize(up);
	glm::vec3 zUnit = glm::normalize(lookAt - lookFrom);

	glm::vec3 delta2 = delta.x * xUnit + delta.y * yUnit + delta.z * zUnit;

	lookAt += delta2;
	lookFrom += delta2;

	update();
}

const glm::mat4& Camera::getMatrix() const {
	return matrix;
}

const glm::vec3& Camera::getLookFrom() const {
	return lookFrom;
}

const glm::vec3& Camera::getLookAt() const {
	return lookAt;
}

const glm::vec3& Camera::getUp() const {
	return up;
}
