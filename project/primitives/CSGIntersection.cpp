#include "CSGIntersection.hpp"

#include <glm/ext.hpp>

#include "Mesh.hpp"

CSGIntersection::CSGIntersection(GeometryNode* left, GeometryNode* right)
    : left(left), right(right) {
    assert(dynamic_cast<Mesh*>(left) == nullptr && "CSG doesn't support meshes");
    assert(dynamic_cast<Mesh*>(right) == nullptr && "CSG doesn't support meshes");
}

Intersection2 CSGIntersection::intersect2(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    Intersection2 leftIntersection = left->intersect2(rayOrigin, rayDirection);
    Intersection2 rightIntersection = right->intersect2(rayOrigin, rayDirection);

    if (!leftIntersection.i1.intersected || !rightIntersection.i1.intersected) {
        return {Intersection(), Intersection()};
    }

    double leftFar = glm::distance2(rayOrigin, leftIntersection.i2.point);
    double rightNear = glm::distance2(rayOrigin, rightIntersection.i1.point);
    if (leftFar < rightNear) {
        return {Intersection(), Intersection()};
    }

    double rightFar = glm::distance2(rayOrigin, rightIntersection.i2.point);
    double leftNear = glm::distance2(rayOrigin, leftIntersection.i1.point);
    if (rightFar < leftNear) {
        return {Intersection(), Intersection()};
    }

    if (leftNear < rightNear && rightNear < leftFar) {
        return {rightIntersection.i1, leftIntersection.i2};
    }
    else {
        return {leftIntersection.i1, rightIntersection.i2};
    }
}

