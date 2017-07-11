#pragma once

#include "ConvexSolid.hpp"

class Cylinder : public ConvexSolid {
protected:
    std::vector<Intersection>
    getIntersectionsPostTransform(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) const override;

    bool isInsideTransformed(const glm::dvec3& point) const override;
};



