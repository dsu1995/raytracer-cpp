//#include "CSGDifference.hpp"
//
//#include <glm/ext.hpp>
//
//#include "Mesh.hpp"
//
//
//CSGDifference::CSGDifference(GeometryNode* left, GeometryNode* right)
//    : left(left), right(right) {
//    assert(dynamic_cast<Mesh*>(left) == nullptr && "CSG doesn't support meshes");
//    assert(dynamic_cast<Mesh*>(right) == nullptr && "CSG doesn't support meshes");
//}
//
//LineSegment CSGDifference::intersect2(
//    const glm::dvec3& rayOrigin,
//    const glm::dvec3& rayDirection
//) const {
//    LineSegment leftIntersection = left->intersect2(rayOrigin, rayDirection);
//    LineSegment rightIntersection = right->intersect2(rayOrigin, rayDirection);
//
//    if (!leftIntersection.near.intersected) {
//        return {Intersection(), Intersection()};
//    }
//
//    if (!rightIntersection.near.intersected) {
//        return leftIntersection;
//    }
//
//    double leftFar = glm::distance2(rayOrigin, leftIntersection.far.point);
//    double rightNear = glm::distance2(rayOrigin, rightIntersection.near.point);
//    if (leftFar < rightNear) {
//        return leftIntersection;
//    }
//
//    double rightFar = glm::distance2(rayOrigin, rightIntersection.far.point);
//    double leftNear = glm::distance2(rayOrigin, leftIntersection.near.point);
//    if (rightFar < leftNear) {
//        return leftIntersection;
//    }
//
//    if (leftNear < rightNear && rightNear < leftFar) {
//        return {leftIntersection.near, rightIntersection.near};
//    }
//    else {
//        return {rightIntersection.far, leftIntersection.far};
//    }
//}
