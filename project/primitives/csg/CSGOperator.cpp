#include "CSGOperator.hpp"


Intersection CSGOperator::getClosestIntersection(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<LineSegment> segments = getCSGSegments(rayOrigin, rayDirection);

    if (segments.empty()) {
        return Intersection();
    }
    else {
        return segments.front().near;
    }
}

std::vector<LineSegment> CSGOperator::getCSGSegments(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    Ray transformedRay = transformRay(rayOrigin, rayDirection);
    const glm::dvec3& newOrigin = transformedRay.rayOrigin;
    const glm::dvec3& newDirection = transformedRay.rayDirection;

    std::vector<LineSegment> segments = getCSGSegmentsPostTransform(newOrigin, newDirection);

    std::vector<LineSegment> transformedSegments;
    for (const LineSegment& segment: segments) {
        const Intersection& near = segment.near;
        const Intersection& far = segment.far;

        transformedSegments.push_back(
            LineSegment(
                transformIntersectionBack(near),
                transformIntersectionBack(far)
            )
        );
    }

    return transformedSegments;
}

AABB CSGOperator::getAABB() const {
    AABB leftAABB = left->getAABB();
    AABB rightAABB = right->getAABB();

    glm::dvec3 p1(
        std::min(leftAABB.p1.x, rightAABB.p1.x),
        std::min(leftAABB.p1.y, rightAABB.p1.y),
        std::min(leftAABB.p1.z, rightAABB.p1.z)
    );
    glm::dvec3 p2(
        std::max(leftAABB.p2.x, rightAABB.p2.x),
        std::max(leftAABB.p2.y, rightAABB.p2.y),
        std::max(leftAABB.p2.z, rightAABB.p2.z)
    );

    return AABB(p1, p2, getTransform());
}
