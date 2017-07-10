#pragma once


#include "CSGOperator.hpp"

class CSGUnion : public CSGOperator {
public:
    CSGUnion(GeometryNode* left, GeometryNode* right);

    std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};



