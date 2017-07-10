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
    dmat4 inv(invtrans);
    dvec3 newOrigin(inv * dvec4(rayOrigin, 1));
    dvec3 newDirection(inv * dvec4(rayDirection, 0));

    // intersect with this node
    Intersection closest = m_primitive->intersect(newOrigin, newDirection);
    if (closest.intersected && closest.node == nullptr) {
        closest.node = this;
    }

    // intersect with children
    for (SceneNode* child: children) {
        Intersection intersection = child->intersect(newOrigin, newDirection);
        closest = Intersection::min(newOrigin, closest, intersection);
    }

    if (closest.intersected) {
        closest.point = dvec3(dmat4(trans) * glm::vec4(closest.point, 1));

        dmat3 normalTransform(inv);
        normalTransform = glm::transpose(normalTransform);
        closest.normal = normalTransform * closest.normal;
    }

    return closest;
}

Intersection2 GeometryNode::intersect2(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) {
    dmat4 inv(invtrans);
    dvec3 newOrigin(inv * dvec4(rayOrigin, 1));
    dvec3 newDirection(inv * dvec4(rayDirection, 0));

    // intersect with this node
    Intersection2 intersection2 = m_primitive->intersect2(newOrigin, newDirection);
    if (intersection2.i1.intersected && intersection2.i1.node == nullptr) {
        intersection2.i1.node = this;
    }
    if (intersection2.i2.intersected && intersection2.i2.node == nullptr) {
        intersection2.i2.node = this;
    }

    if (intersection2.i1.intersected) {
        intersection2.i1.point = dvec3(dmat4(trans) * glm::vec4(intersection2.i1.point, 1));

        dmat3 normalTransform(inv);
        normalTransform = glm::transpose(normalTransform);
        intersection2.i1.normal = normalTransform * intersection2.i1.normal;
    }
    if (intersection2.i2.intersected) {
        intersection2.i2.point = dvec3(dmat4(trans) * glm::vec4(intersection2.i2.point, 1));

        dmat3 normalTransform(inv);
        normalTransform = glm::transpose(normalTransform);
        intersection2.i2.normal = normalTransform * intersection2.i2.normal;
    }

    return intersection2;
}
