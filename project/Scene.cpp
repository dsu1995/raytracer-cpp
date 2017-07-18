#include <glm/ext.hpp>
#include "Scene.hpp"
#include "primitives/solid/Cube.hpp"

using glm::dvec3;
using glm::dmat3;
using glm::dvec4;
using glm::dmat4;

const bool GRID_ENABLED = true;


Scene::Scene(SceneNode* root)
    : root(root) {
    if (!GRID_ENABLED) return;

    flatten(root, dmat4());

    grid = Grid(flattenedPrimitives);
}

Scene::~Scene() {
    for (Primitive* primitive: flattenedPrimitives) {
        delete primitive;
    }
}

void Scene::flatten(SceneNode* node, dmat4 T) {
    T *= node->getTransform();

    GeometryNode* const gnode = dynamic_cast<GeometryNode*>(node);
    if (gnode != nullptr) {
        Primitive* primitive = gnode->m_primitive->clone();
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
    Intersection i = root->intersect(rayOrigin, rayDirection);
    Intersection i2 = grid.trace(rayOrigin, rayDirection);

    assert(i.intersected == i2.intersected && i.material.m_kd == i2.material.m_kd);
    return i;

//    if (i.intersected != i2.intersected || i.material.m_kd != i2.material.m_kd) {
//        return grid.trace(rayOrigin, rayDirection);
//    }
//    else {
//        return i;
//    }
}

//PhongMaterial dummyMaterial3(dvec3(1, 0, 0), dvec3(1, 0, 0), 1, 0, 0, 0, 0);
//
//Intersection Scene::trace(
//    const dvec3& rayOrigin,
//    const dvec3& rayDirection
//) const {
//    if (!GRID_ENABLED) {
//        return root->intersect(rayOrigin, rayDirection);
//    }
//    else {
//        return grid.trace(rayOrigin, rayDirection);
////        Intersection closest;
////        for (Primitive* primitive: flattenedPrimitives) {
////            AABB aabb = primitive->getAABB();
////            Cube cube(aabb.p1, aabb.p2 - aabb.p1);
////            cube.setMaterial(&dummyMaterial3);
////
////            Intersection intersection =
////                primitive->getClosestIntersection(rayOrigin, rayDirection);
////
////            if (!intersection.intersected) {
////                intersection = cube.getClosestIntersection(rayOrigin, rayDirection);
////            }
////            closest = Intersection::min(
////                rayOrigin,
////                closest,
////                intersection
////            );
////        }
////        return closest;
//    }
//}


bool Scene::existsObjectBetween(
    const dvec3& p1,
    const dvec3& p2
) const {
    Intersection intersection = trace(p1, p2 - p1);
    return intersection.intersected &&
           glm::distance2(p1, intersection.point) <= glm::distance2(p1, p2);
}
