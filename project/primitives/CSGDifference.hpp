#pragma once


#include "CSGOperator.hpp"

class CSGDifference : public CSGOperator {
public:
    CSGDifference(GeometryNode* left, GeometryNode* right);

    std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};



