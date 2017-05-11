#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace cube {
	const std::vector<glm::vec3> VERTICES = {
		{0, 0, 0},
		{0, 1, 0},
		{0, 1, 1},
		{0, 0, 1},
		{1, 0, 1},
		{1, 0, 0},
		{1, 1, 0},
		{1, 1, 1},
	};

	const std::vector<GLubyte> TRIANGLES = {
		0,1,2, 2,3,0,
		0,3,4, 4,5,0,
		0,5,6, 6,1,0,
		1,6,7, 7,2,1,
		7,4,3, 3,2,7,
		4,7,6, 6,5,4
	};
}
