#pragma once

#include "ConvexSolid.hpp"

class Sphere : public ConvexSolid {
public:
    Sphere();

    Sphere(const glm::vec3& pos, double radius);

    const glm::vec3 m_pos;
    const double m_radius;

    Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};

