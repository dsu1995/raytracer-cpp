#include "CSGDifference.hpp"

#include <glm/ext.hpp>

#include "Mesh.hpp"


CSGDifference::CSGDifference(GeometryNode* left, GeometryNode* right)
    : left(left), right(right) {
    assert(dynamic_cast<Mesh*>(left) == nullptr && "CSG doesn't support meshes");
    assert(dynamic_cast<Mesh*>(right) == nullptr && "CSG doesn't support meshes");
}

Intersection2 CSGDifference::intersect2(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    Intersection2 leftIntersection = left->intersect2(rayOrigin, rayDirection);
    Intersection2 rightIntersection = right->intersect2(rayOrigin, rayDirection);

    if (!leftIntersection.i1.intersected) {
        return {Intersection(), Intersection()};
    }

    if (!rightIntersection.i1.intersected) {
        return leftIntersection;
    }

    double leftFar = glm::distance2(rayOrigin, leftIntersection.i2.point);
    double rightNear = glm::distance2(rayOrigin, rightIntersection.i1.point);
    if (leftFar < rightNear) {
        return leftIntersection;
    }

    double rightFar = glm::distance2(rayOrigin, rightIntersection.i2.point);
    double leftNear = glm::distance2(rayOrigin, leftIntersection.i1.point);
    if (rightFar < leftNear) {
        return leftIntersection;
    }

    if (leftNear < rightNear && rightNear < leftFar) {
        return {leftIntersection.i1, rightIntersection.i1};
    }
    else {
        return {rightIntersection.i2, leftIntersection.i2};
    }
}
