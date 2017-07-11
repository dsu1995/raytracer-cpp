#include "ConvexSolid.hpp"
#include <algorithm>
#include <glm/ext.hpp>

Intersection ConvexSolid::getClosestIntersection(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<Intersection> intersections =
        getIntersections(rayOrigin, rayDirection);

    if (intersections.empty()) {
        return Intersection();
    }
    else if (intersections.size() == 1 || intersections.size() == 2) {
        return intersections.front();
    }
    else {
        assert(false && "Convex Solid has more than 2 intersections!");
    }
}

std::vector<LineSegment>
ConvexSolid::getCSGSegments(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<Intersection> intersections =
        getIntersections(rayOrigin, rayDirection);

    if (intersections.empty()) {
        return {};
    }
    else if (intersections.size() == 1) {
        if (isInside(rayOrigin)) {
            return {
                LineSegment(
                    Intersection(rayOrigin, -rayDirection, this),
                    intersections.front()
                )
            };
        }
        else {
            return {};
        }
    }
    else if (intersections.size() == 2) {
        return {
            LineSegment(intersections.at(0), intersections.at(1))
        };
    }
    else {
        assert(false && "Convex Solid has more than 2 intersections!");
    }
}

const double EPS = 0.0000001;

std::vector<Intersection> ConvexSolid::getIntersections(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    Ray transformedRay = transformRay(rayOrigin, rayDirection);
    const glm::dvec3& newOrigin = transformedRay.rayOrigin;
    const glm::dvec3& newDirection = transformedRay.rayDirection;

    std::vector<Intersection> intersections =
        getIntersectionsPostTransform(newOrigin, newDirection);

    std::sort(
        intersections.begin(), intersections.end(),
        [&newOrigin](const Intersection& a, const Intersection& b) {
            return glm::distance2(newOrigin, a.point) <
                   glm::distance2(newOrigin, b.point);
        }
    );

    // remove 2 intersections from 1 point
    // due to floating point errors
    if (intersections.size() > 2) {
        for (size_t i = 1; i < intersections.size();) {
            double dist = glm::distance(intersections.at(i).point, intersections.at(i - 1).point);

            if (fabs(dist) < EPS) {
                intersections.erase(intersections.begin() + i);
            }
            else {
                i++;
            }
        }
    }

    std::vector<Intersection> transformedIntersections;
    for (const Intersection& intersection: intersections) {
        transformedIntersections.push_back(
            transformIntersectionBack(intersection)
        );
    }

    return transformedIntersections;
}
