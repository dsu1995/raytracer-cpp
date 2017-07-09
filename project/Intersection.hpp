#pragma once

#include <glm/glm.hpp>

class GeometryNode;

struct Intersection {
    bool intersected;
    glm::dvec3 point;
    glm::dvec3 normal;
    GeometryNode* node;

    Intersection();

    Intersection(
        const glm::dvec3& point,
        const glm::dvec3& normal,
        GeometryNode* node = nullptr
    );

    static const Intersection& min(
        const glm::dvec3& rayOrigin,
        const Intersection& i1,
        const Intersection& i2
    );
};

