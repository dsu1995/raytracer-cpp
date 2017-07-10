#include "GeometryNode.hpp"

using glm::dvec3;
using glm::dvec4;
using glm::dmat3;
using glm::dmat4;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
    const std::string& name,
    Primitive* prim
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
