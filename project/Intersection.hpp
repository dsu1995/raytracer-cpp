#pragma once

#include <glm/glm.hpp>
#include "PhongMaterial.hpp"

class Primitive;

struct Intersection {
    bool intersected;
    glm::dvec3 point;
    glm::dvec3 normal;
    glm::dvec3 objCenter;
    PhongMaterial material;

    Intersection();

    Intersection(
        const glm::dvec3& point,
        const glm::dvec3& normal,
        const glm::dvec3& objCenter,
        const PhongMaterial& node
    );

    static const Intersection& min(
        const glm::dvec3& rayOrigin,
        const Intersection& i1,
        const Intersection& i2
    );
};

struct LineSegment {
    Intersection near;
    Intersection far;

    LineSegment();
    LineSegment(const Intersection& near, const Intersection& far);

    bool isEmpty() const;
};
