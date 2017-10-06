#pragma once


#include "CSGOperator.hpp"

class CSGDifference : public CSGOperator {
public:
    CSGDifference(GeometryNode* left, GeometryNode* right);

    virtual std::vector<LineSegment> getCSGSegmentsPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    virtual CSGDifference* clone() const override;

protected:
    virtual bool isInsideTransformed(const glm::dvec3& point) const override;
};



