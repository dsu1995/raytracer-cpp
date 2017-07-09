#pragma once

#include <glm/glm.hpp>
#include "../Intersection.hpp"


class Primitive {
public:
    virtual ~Primitive() {}
    virtual Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;
};
