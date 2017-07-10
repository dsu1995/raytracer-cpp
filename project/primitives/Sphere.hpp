#pragma once

#include "Primitive.hpp"

class Sphere : public Primitive {
public:
    Sphere();

    Sphere(const glm::vec3& pos, double radius);

    const glm::vec3 m_pos;
    const double m_radius;

    std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};

