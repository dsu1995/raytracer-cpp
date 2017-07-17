#pragma once


#include "ConvexSolid.hpp"

class Cone : public ConvexSolid {
public:
    virtual AABB getAABB() const override;

    Cone* clone() const override;

protected:
    virtual std::vector<Intersection> getIntersectionsPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const;

    bool isInsideTransformed(const glm::dvec3& point) const override;
};



