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
            dvec3 p = rayOrigin + t * rayDirection;
            Intersection intersection(p, getNormal(p), m_pos, getMaterial(p));
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

        return newMaterial;
    }
}

const dvec3 up(0, 1, 0);

glm::dvec3 Sphere::getNormal(const glm::dvec3& point) const {
    if (normalMap == nullptr) {
        return point - m_pos;
    }
    else {
        dvec3 d = glm::normalize(m_pos - point);

        double u = 0.5 - atan2(d.z, d.x) / (2 * M_PI);
        double v = 0.5 + asin(d.y) / M_PI;

        dvec3 normalOffset = normalMap->getNormalOffset(u, v);

        dvec3 uhat = glm::cross(d, up);
        dvec3 vhat = glm::cross(uhat, d);

        return normalOffset.x * uhat + normalOffset.y * vhat + normalOffset.z * (-d);
    }
}

AABB Sphere::getAABB() const {
    return AABB(
        m_pos - dvec3(m_radius),
        m_pos + dvec3(m_radius),
        getTransform()
    );
}
