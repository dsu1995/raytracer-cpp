#include <glm/ext.hpp>
#include "Scene.hpp"

using glm::dvec3;
using glm::dmat3;
using glm::dvec4;
using glm::dmat4;


Scene::Scene(SceneNode* root)
    : root(root) {}

Scene::~Scene() {}


bool Scene::existsObjectBetween(
    const dvec3& p1,
    const dvec3& p2
) const {
    Intersection intersection = trace(p1, p2 - p1);
    return intersection.intersected &&
        glm::distance2(p1, intersection.point) <= glm::distance2(p1, p2);
}


Intersection Scene::trace(
    const dvec3& rayOrigin,
    const dvec3& rayDirection
) const {
    return root->intersect(rayOrigin, rayDirection);
}
