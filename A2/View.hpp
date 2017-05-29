#pragma once

#include <glm/glm.hpp>


class View {
public:
	View();
	~View();

	void rotateX(float radians);
	void rotateY(float radians);
	void rotateZ(float radians);

	void translate(const glm::vec3& delta);

	const glm::mat4& getMatrix() const;

	void reset();

private:
	glm::mat4 matrix;
};
