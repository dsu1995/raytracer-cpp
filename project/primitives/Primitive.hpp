#pragma once

#include <glm/glm.hpp>
#include "../Intersection.hpp"

class Primitive {
public:
    virtual ~Primitive() {}
    virtual Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const {
        Intersection2 intersection2 = intersect2(rayOrigin, rayDirection);
        return Intersection::min(rayOrigin, intersection2.i1, intersection2.i2);
    }

    virtual Intersection2 intersect2(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;
};
