#pragma once

#include <glm/glm.hpp>

#include "nodes/SceneNode.hpp"
#include "nodes/GeometryNode.hpp"



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
    SceneNode* const root;

    Intersection traceRecursive(
        SceneNode* node,
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const;
};



