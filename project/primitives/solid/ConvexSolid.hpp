#pragma once

#include "../Primitive.hpp"

class ConvexSolid : public Primitive {
public:
    Intersection getClosestIntersection(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    std::vector<LineSegment> getCSGSegments(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    virtual AABB getAABB() const = 0;

protected:
    virtual std::vector<Intersection> getIntersectionsPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;

    virtual bool isInsideTransformed(const glm::dvec3& point) const = 0;

private:
    std::vector<Intersection> getIntersections(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const;
};