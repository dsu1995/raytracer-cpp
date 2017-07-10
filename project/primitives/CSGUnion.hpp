#pragma once


#include "CSGOperator.hpp"

class CSGUnion : public CSGOperator {
public:
    CSGUnion(GeometryNode* left, GeometryNode* right);

    Intersection2 intersect2(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

private:
    GeometryNode* left;
    GeometryNode* right;
};



