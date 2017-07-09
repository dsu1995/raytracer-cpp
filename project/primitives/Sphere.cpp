#include "Sphere.hpp"
#include <glm/ext.hpp>

#include "../polyroots.hpp"

using glm::vec3;
using glm::dvec3;

Sphere::~Sphere() {}

Sphere::Sphere(const vec3& pos, double radius)
    : m_pos(pos), m_radius(radius) {}

Sphere::Sphere() : Sphere(vec3(), 1.0) {}


Intersection Sphere::intersect(
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

    double t;
    if (numRoots == 0) {
        return closest;
    }
    else if (numRoots == 1) {
        t = roots[0];
        if (t < 0) {
            return closest;
        }
    }
    else if (numRoots == 2) {
        double t1 = roots[0];
        double t2 = roots[1];
        if (t1 < 0 && t2 < 0) {
            return closest;
        }
        else if (t1 >= 0 && t2 >= 0) {
            t = std::min(t1, t2);
        }
        else if (t1 >= 0 && t2 < 0) {
            t = t1;
        }
        else if (t1 < 0 && t2 >= 0) {
            t = t2;
        }
        else {
            assert(false);
        }
    }
    else {
        assert(false);
    }

    dvec3 intersection = rayOrigin + t * rayDirection;
    dvec3 normal = intersection - c;
    return {intersection, normal};
}
