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

    void setMaterial(PhongMaterial* material);

    Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    );

    PhongMaterial* m_material;
    Primitive* m_primitive;
};