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


std::vector<LineSegment> Sphere::allIntersectPostTransform(
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
            Intersection intersection(intersectionPoint, normal, this);
            intersections.push_back(intersection);
        }
    }

    if (intersections.size() == 0 || intersections.size() == 1) {
        return {};
    }
    else if (intersections.size() == 2) {
        std::sort(
            intersections.begin(), intersections.end(),
            [&rayOrigin](const Intersection& a, const Intersection& b) {
                return glm::distance2(rayOrigin, a.point) <
                       glm::distance2(rayOrigin, b.point);
            }
        );
        return {
            LineSegment(intersections.at(0), intersections.at(1))
        };
    }
    else {
        assert(false && "Sphere should have between 0 and 2 intersections with ray.");
    }
}
