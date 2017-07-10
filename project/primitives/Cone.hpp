#pragma once


#include "Primitive.hpp"

class Cone : public Primitive {
public:
    Intersection2 intersect2(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};



