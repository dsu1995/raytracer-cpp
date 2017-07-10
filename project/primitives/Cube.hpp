#pragma once

#include "Primitive.hpp"

class Cube : public Primitive {
public:
    Cube(const glm::vec3& pos, double size);

    Cube(const glm::vec3& pos, const glm::vec3& dims);

    Cube();

    std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    bool isInside(const glm::dvec3 point) const;

    const glm::vec3 m_pos;
    const glm::dvec3 dims;
};


