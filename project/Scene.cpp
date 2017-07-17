#include <glm/ext.hpp>
#include "Scene.hpp"

using glm::dvec3;
using glm::dmat3;
using glm::dvec4;
using glm::dmat4;

const bool GRID_ENABLED = true;


Scene::Scene(SceneNode* root)
    : root(root) {
    flatten(root, dmat4());
}

Scene::~Scene() {}

void Scene::flatten(SceneNode* node, dmat4 T) {
    if (!GRID_ENABLED) return;

    T *= node->getTransform();

    GeometryNode* const gnode = dynamic_cast<GeometryNode*>(node);
    if (gnode != nullptr) {
        Primitive* primitive = gnode->m_primitive;
        dmat4 nodeT = T * primitive->getTransform();
        primitive->setTransform(nodeT, glm::inverse(nodeT));
        flattenedPrimitives.push_back(primitive);
    }

    for (SceneNode* child: node->children) {
        flatten(child, T);
    }
}


Intersection Scene::trace(
    const dvec3& rayOrigin,
    const dvec3& rayDirection
) const {
    if (!GRID_ENABLED) {
        return root->intersect(rayOrigin, rayDirection);
    }
    else {
        Intersection closest;
        for (Primitive* primitive: flattenedPrimitives) {
            closest = Intersection::min(
                rayOrigin,
                closest,
                primitive->getClosestIntersection(rayOrigin, rayDirection)
            );
        }
        return closest;
    }
}


bool Scene::existsObjectBetween(
    const dvec3& p1,
    const dvec3& p2
) const {
    Intersection intersection = trace(p1, p2 - p1);
    return intersection.intersected &&
           glm::distance2(p1, intersection.point) <= glm::distance2(p1, p2);
}
