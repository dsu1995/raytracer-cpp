#include "CSGUnion.hpp"

CSGUnion::CSGUnion(GeometryNode* left, GeometryNode* right)
    : left(left), right(right) {}

Intersection CSGUnion::intersect(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    Intersection leftIntersection = left->intersect(rayOrigin, rayDirection);
    Intersection rightIntersection = right->intersect(rayOrigin, rayDirection);
    return Intersection::min(rayOrigin, leftIntersection, rightIntersection);
}
