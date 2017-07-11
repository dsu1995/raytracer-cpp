#pragma once


#include "CSGOperator.hpp"

class CSGUnion : public CSGOperator {
public:
    CSGUnion(GeometryNode* left, GeometryNode* right);

    virtual std::vector<LineSegment> getCSGSegmentsPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

protected:
    virtual bool isInsideTransformed(const glm::dvec3& point) const override;
};



