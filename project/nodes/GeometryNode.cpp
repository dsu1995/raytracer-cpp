#include "GeometryNode.hpp"

using glm::dvec3;
using glm::dvec4;
using glm::dmat3;
using glm::dmat4;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
    const std::string& name,
    Primitive* prim,
    PhongMaterial* mat
) : SceneNode(name),
    m_primitive(prim) {
    m_nodeType = NodeType::GeometryNode;
}

Intersection GeometryNode::intersect(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) {
    Ray transformedRay = transformRay(rayOrigin, rayDirection);
    const dvec3& newOrigin = transformedRay.rayOrigin;
    const dvec3& newDirection = transformedRay.rayDirection;

    Intersection closest = m_primitive->closestIntersect(newOrigin, newDirection);

    for (SceneNode* child: children) {
        Intersection intersection = child->intersect(newOrigin, newDirection);
        closest = Intersection::min(newOrigin, closest, intersection);
    }

    return transformIntersectionBack(closest);
}

//LineSegment GeometryNode::intersect2(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) {
//    dmat4 inv(invtrans);
//    dvec3 newOrigin(inv * dvec4(rayOrigin, 1));
//    dvec3 newDirection(inv * dvec4(rayDirection, 0));
//
//    // intersect with this node
//    LineSegment intersection2 = m_primitive->allIntersect(newOrigin, newDirection);
//
//    if (intersection2.near.intersected) {
//        intersection2.near.point = dvec3(dmat4(trans) * glm::vec4(intersection2.near.point, 1));
//
//        dmat3 normalTransform(inv);
//        normalTransform = glm::transpose(normalTransform);
//        intersection2.near.normal = normalTransform * intersection2.near.normal;
//    }
//    if (intersection2.far.intersected) {
//        intersection2.far.point = dvec3(dmat4(trans) * glm::vec4(intersection2.far.point, 1));
//
//        dmat3 normalTransform(inv);
//        normalTransform = glm::transpose(normalTransform);
//        intersection2.far.normal = normalTransform * intersection2.far.normal;
//    }
//
//    return intersection2;
//}
