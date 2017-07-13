#pragma once

#include <glm/glm.hpp>

class Primitive;

struct Intersection {
    bool intersected;
    glm::dvec3 point;
    glm::dvec3 normal;
    glm::dvec3 objCenter;
    const Primitive* primitive;

    Intersection();

    Intersection(
        const glm::dvec3& point,
        const glm::dvec3& normal,
        const glm::dvec3& objCenter,
        const Primitive* node
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
