#pragma once


#include "Primitive.hpp"

class Cone : public Primitive {
public:
    Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};



