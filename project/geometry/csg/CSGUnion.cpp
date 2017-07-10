#include "CSGUnion.hpp"

using glm::dvec3;

CSGUnion::CSGUnion(CSGGeometry* left, CSGGeometry* right)
    : left(left), right(right) {}

Intersection CSGUnion::intersect(
    const dvec3& rayOrigin,
    const dvec3& rayDirection
) const {
    Intersection leftIntersection = left->intersect(rayOrigin, rayDirection);
    Intersection rightIntersection = right->intersect(rayOrigin, rayDirection);
    return Intersection::min(rayOrigin, leftIntersection, rightIntersection);
}
