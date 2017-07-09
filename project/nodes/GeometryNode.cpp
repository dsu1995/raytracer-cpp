#include "GeometryNode.hpp"

using glm::dvec3;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
    const std::string& name,
    Primitive* prim,
    PhongMaterial* mat
) : SceneNode(name),
    m_material(mat),
    m_primitive(prim) {
    m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial(PhongMaterial* mat) {
    // Obviously, there's a potential memory leak here.  A good solution
    // would be to use some kind of reference counting, as in the
    // C++ shared_ptr.  But I'm going to punt on that problem here.
    // Why?  Two reasons:
    // (a) In practice we expect the scene to be constructed exactly
    //     once.  There's no reason to believe that materials will be
    //     repeatedly overwritten in a GeometryNode.
    // (b) A ray tracer is a program in which you compute once, and
    //     throw away all your data.  A memory leak won't build up and
    //     crash the program.

    m_material = mat;
}

Intersection GeometryNode::intersect(
    const dvec3& rayOrigin,
    const dvec3& rayDirection
) {
    Intersection intersection = m_primitive->intersect(rayOrigin, rayDirection);
    intersection.node = this;
    return intersection;
}
