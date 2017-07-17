#pragma once

#include <glm/glm.hpp>

#include "nodes/SceneNode.hpp"
#include "nodes/GeometryNode.hpp"
#include "Grid.hpp"


class Scene {
public:
    Scene(SceneNode* root);
    virtual ~Scene();

    bool existsObjectBetween(
        const glm::dvec3& p1,
        const glm::dvec3& p2
    ) const;

    Intersection trace(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const;

private:
    void flatten(SceneNode* node, glm::dmat4 T);

    SceneNode* const root;

    std::vector<Primitive*> flattenedPrimitives;

    Grid grid;
};



