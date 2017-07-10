#pragma once

#include "Primitive.hpp"

class Cube : public Primitive {
public:
    Cube(const glm::vec3& pos, double size);

    Cube(const glm::vec3& pos, const glm::vec3& dims);

    Cube();

    Intersection2 intersect2(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    const glm::vec3 m_pos;
    const glm::dvec3 dims;
};


