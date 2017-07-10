#pragma once

#include "CSGOperator.hpp"

class CSGUnion : public CSGOperator {
public:
    CSGUnion(CSGGeometry* left, CSGGeometry* right);

private:
    Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    CSGGeometry* left;
    CSGGeometry* right;
};



