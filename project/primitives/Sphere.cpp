#include "Sphere.hpp"
#include <glm/ext.hpp>
#include <vector>
#include <algorithm>

#include "../polyroots.hpp"

using glm::vec3;
using glm::dvec3;

Sphere::Sphere(const vec3& pos, double radius)
    : m_pos(pos), m_radius(radius) {}

Sphere::Sphere() : Sphere(vec3(), 1.0) {}


Intersection2 Sphere::intersect2(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    Intersection closest;

    const dvec3& c = m_pos;
    double r = m_radius;
    double A = glm::length2(rayDirection);
    double B = glm::dot(rayDirection, rayOrigin - c) * 2;
    double C = glm::length2(rayOrigin - c) - r * r;

    double roots[2];
    size_t numRoots = quadraticRoots(A, B, C, roots);

    std::vector<Intersection> intersections;
    for (size_t i = 0; i < numRoots; i++) {
        double t = roots[i];
        if (t >= 0) {
            dvec3 intersectionPoint = rayOrigin + t * rayDirection;
            dvec3 normal = intersectionPoint - c;
            Intersection intersection(intersectionPoint, normal);
            intersections.push_back(intersection);
        }
    }

    Intersection2 intersection2{Intersection(), Intersection()};
    if (intersections.size() == 0) { ;
    }
    else if (intersections.size() == 1) {
        intersection2.i1 = intersection2.i2 = intersections.at(0);
    }
    else if (intersections.size() == 2) {
        std::sort(
            intersections.begin(), intersections.end(),
            [](const Intersection& a, const Intersection& b) {
                return glm::distance2(rayOrigin, a.point) <
                       glm::distance2(rayOrigin, b.point);
            }
        );

        intersection2.i1 = intersections.at(0);
        intersection2.i2 = intersections.at(1);
    }
    else {
        assert(false);
    }

    return intersection2;
}
