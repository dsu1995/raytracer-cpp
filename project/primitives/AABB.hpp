#pragma once

#include <glm/glm.hpp>

/**
 * Axis Aligned Bounding Box
 */
struct AABB {
    AABB(const glm::dvec3& p1, const glm::dvec3& p2);
    AABB(const glm::dvec3& p1, const glm::dvec3& p2, const glm::dmat4 T);

    void transform(const glm::dmat4& T);

    bool intersects(const AABB& other) const;

    glm::dvec3 p1;
    glm::dvec3 p2;
};



