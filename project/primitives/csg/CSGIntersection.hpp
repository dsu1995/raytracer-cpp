#pragma once


#include "CSGOperator.hpp"

class CSGIntersection : public CSGOperator {
public:
    CSGIntersection(GeometryNode* left, GeometryNode* right);

    virtual std::vector<LineSegment> getCSGSegmentsPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

protected:
    virtual bool isInsideTransformed(const glm::dvec3& point) const override;
};




