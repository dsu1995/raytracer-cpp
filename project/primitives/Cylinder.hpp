#pragma once

#include "Primitive.hpp"

class Cylinder : public Primitive {
public:
    std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};



