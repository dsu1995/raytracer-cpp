#pragma once


#include "CSGOperator.hpp"

class CSGIntersection : public CSGOperator {
public:
    CSGIntersection(GeometryNode* left, GeometryNode* right);

    std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;
};




