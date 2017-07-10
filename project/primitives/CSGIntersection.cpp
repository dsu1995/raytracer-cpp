//#include "CSGIntersection.hpp"
//
//#include <glm/ext.hpp>
//
//#include "Mesh.hpp"
//
//CSGIntersection::CSGIntersection(GeometryNode* left, GeometryNode* right)
//    : left(left), right(right) {
//    assert(dynamic_cast<Mesh*>(left) == nullptr && "CSG doesn't support meshes");
//    assert(dynamic_cast<Mesh*>(right) == nullptr && "CSG doesn't support meshes");
//}
//
//LineSegment CSGIntersection::intersect2(
//    const glm::dvec3& rayOrigin,
//    const glm::dvec3& rayDirection
//) const {
//    LineSegment leftIntersection = left->intersect2(rayOrigin, rayDirection);
//    LineSegment rightIntersection = right->intersect2(rayOrigin, rayDirection);
//
//    if (!leftIntersection.near.intersected || !rightIntersection.near.intersected) {
//        return {Intersection(), Intersection()};
//    }
//
//    double leftFar = glm::distance2(rayOrigin, leftIntersection.far.point);
//    double rightNear = glm::distance2(rayOrigin, rightIntersection.near.point);
//    if (leftFar < rightNear) {
//        return {Intersection(), Intersection()};
//    }
//
//    double rightFar = glm::distance2(rayOrigin, rightIntersection.far.point);
//    double leftNear = glm::distance2(rayOrigin, leftIntersection.near.point);
//    if (rightFar < leftNear) {
//        return {Intersection(), Intersection()};
//    }
//
//    if (leftNear < rightNear && rightNear < leftFar) {
//        return {rightIntersection.near, leftIntersection.far};
//    }
//    else {
//        return {leftIntersection.near, rightIntersection.far};
//    }
//}
//
