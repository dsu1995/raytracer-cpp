#include "Sphere.hpp"
#include <glm/ext.hpp>
#include <vector>
#include <algorithm>

#include "../../polyroots.hpp"

using glm::dvec3;

Sphere::Sphere(const dvec3& pos, double radius)
    : m_pos(pos), m_radius(radius) {}

Sphere::Sphere() : Sphere(dvec3(), 1.0) {}


std::vector<Intersection>
Sphere::getIntersectionsPostTransform(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
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
            Intersection intersection(intersectionPoint, normal, m_pos, getMaterial(intersectionPoint));
            intersections.push_back(intersection);
        }
    }

    return intersections;
}


bool Sphere::isInsideTransformed(const glm::dvec3& point) const {
    return glm::distance2(m_pos, point) <= m_radius * m_radius;
}

PhongMaterial Sphere::getMaterial(const glm::dvec3& point) const {
    if (texture == nullptr) {
        return *material;
    }
    else {
        PhongMaterial newMaterial = *material;

        // https://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere
        dvec3 d = glm::normalize(m_pos - point);

        double u = 0.5 - atan2(d.z, d.x) / (2 * M_PI);
        double v = 0.5 + asin(d.y) / M_PI;
        newMaterial.m_kd = texture->getPixel(u, v);

//        dvec3 vector = point - m_pos;
//
//        double theta = acos(vector.z / m_radius); // [0, PI]
//        double phi = atan2(vector.y, vector.x); // [-PI, PI]
//
//        newMaterial.m_kd = texture->getPixel(theta / M_PI, phi / (2 * M_PI) + 0.5);
        return newMaterial;
    }
}
