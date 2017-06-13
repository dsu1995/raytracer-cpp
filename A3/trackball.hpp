#pragma once

#include <glm/glm.hpp>

namespace trackball {
    glm::vec3 calculateRotationVector(
        const glm::vec2& newVec,
        const glm::vec2& oldVec,
        float diameter
    );

    glm::mat4 rotationAroundAxisMatrix(
        float radians,
        const glm::vec3& axis
    );
}