#pragma once

#include "CSGOperator.hpp"

class CSGIntersection : public CSGOperator {
public:
    CSGIntersection(GeometryNode* left, GeometryNode* right);

    Intersection2 intersect2(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

private:
    GeometryNode* left;
    GeometryNode* right;
};



