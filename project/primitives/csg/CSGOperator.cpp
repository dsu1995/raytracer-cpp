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
