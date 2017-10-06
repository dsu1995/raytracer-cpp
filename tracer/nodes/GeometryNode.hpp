#pragma once

#include "SceneNode.hpp"
#include "../primitives/Primitive.hpp"
#include "../PhongMaterial.hpp"

class GeometryNode : public SceneNode {
public:
    GeometryNode(
        const std::string& name,
        Primitive* prim
    );

    Primitive* const m_primitive;

protected:
    virtual Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    );
};
