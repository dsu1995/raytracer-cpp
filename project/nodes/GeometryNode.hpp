#pragma once

#include "SceneNode.hpp"
#include "../primitives/Primitive.hpp"
#include "../PhongMaterial.hpp"

class GeometryNode : public SceneNode {
public:
    GeometryNode(
        const std::string& name,
        Primitive* prim,
        PhongMaterial* mat = nullptr
    );

    Primitive* const m_primitive;


protected:
    virtual Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    );

//    virtual LineSegment intersect2(
//        const glm::dvec3& rayOrigin,
//        const glm::dvec3& rayDirection
//    );

};
