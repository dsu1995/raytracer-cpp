#pragma once

#include "ConvexSolid.hpp"

class Cone : public ConvexSolid {
public:
    Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};
