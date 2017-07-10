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
//    return traceRecursive(root, rayOrigin, rayDirection);
    return root->intersect(rayOrigin, rayDirection);
}

//Intersection Scene::traceRecursive(
//    SceneNode* node,
//    const dvec3& rayOrigin,
//    const dvec3& rayDirection
//) const {
//    dmat4 inv = node->get_inverse();
//    dvec3 newOrigin(inv * dvec4(rayOrigin, 1));
//    dvec3 newDirection(inv * dvec4(rayDirection, 0));
//
//    Intersection closest;
//
//    GeometryNode* const gnode = dynamic_cast<GeometryNode*>(node);
//    if (gnode != nullptr) {
//        Intersection intersection = gnode->intersect(newOrigin, newDirection);
//        closest = Intersection::min(newOrigin, closest, intersection);
//    }
//
//    for (SceneNode* child: node->children) {
//        Intersection intersection = traceRecursive(child, newOrigin, newDirection);
//        closest = Intersection::min(newOrigin, closest, intersection);
//    }
//
//    if (closest.intersected) {
//        closest.point = dvec3(node->get_transform() * glm::vec4(closest.point, 1));
//
//        dmat3 normalTransform(node->get_inverse());
//        normalTransform = glm::transpose(normalTransform);
//        closest.normal = normalTransform * closest.normal;
//    }
//
//    return closest;
//}
