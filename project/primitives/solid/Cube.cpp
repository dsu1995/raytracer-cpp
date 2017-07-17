#include "Cube.hpp"
#include <vector>
#include <algorithm>
#include <glm/ext.hpp>
#include <cmath>

// debug
#include <iostream>

using std::cerr;
using std::endl;

using glm::dvec3;

Cube::Cube(const dvec3& pos, const dvec3& dims)
    : m_pos(pos), dims(dims) {}

Cube::Cube(const dvec3& pos, double size)
    : Cube(pos, dvec3(size)) {}

Cube::Cube()
    : Cube(dvec3(0), 1) {}


std::vector<Intersection> Cube::getIntersectionsPostTransform(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<Intersection> intersections;

    const glm::dvec3& e = rayOrigin;
    const glm::dvec3& d = rayDirection;

    double x_min = m_pos.x;
    double x_max = m_pos.x + dims.x;

    double y_min = m_pos.y;
    double y_max = m_pos.y + dims.y;

    double z_min = m_pos.z;
    double z_max = m_pos.z + dims.z;

    { // left face
        double t = (x_min - e.x) / d.x;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((y_min <= p.y && p.y <= y_max) &&
                (z_min <= p.z && p.z <= z_max)) {
                dvec3 d2 = p - m_pos;

                PhongMaterial newMaterial = *material;
                if (texture != nullptr) {
                    newMaterial.m_kd = texture->getPixel(d2.z / dims.z, 1 - d2.y / dims.y);
                }

                dvec3 normal(-1, 0, 0);
                if (normalMap != nullptr) {
                    dvec3 normalOffset = normalMap->getNormalOffset(d2.z / dims.z, 1 - d2.y / dims.y);
                    normal = dvec3(
                        -normalOffset.z,
                        normalOffset.y,
                        normalOffset.x
                    );
                }

                intersections.push_back(
                    Intersection(p, normal, objCenter(), newMaterial)
                );
            }
        }
    }

    { // right face
        double t = (x_max - e.x) / d.x;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((y_min <= p.y && p.y <= y_max) &&
                (z_min <= p.z && p.z <= z_max)) {
                dvec3 d2 = p - m_pos;

                PhongMaterial newMaterial = *material;
                if (texture != nullptr) {
                    newMaterial.m_kd = texture->getPixel(1 - d2.z / dims.z, 1 - d2.y / dims.y);
                }

                dvec3 normal(1, 0, 0);
                if (normalMap != nullptr) {
                    dvec3 normalOffset = normalMap->getNormalOffset(1 - d2.z / dims.z, 1 - d2.y / dims.y);
                    normal = dvec3(
                        normalOffset.z,
                        normalOffset.y,
                        -normalOffset.x
                    );
                }

                intersections.push_back(
                    Intersection(p, normal, objCenter(), newMaterial)
                );
            }
        }
    }

    { // bottom face
        double t = (y_min - e.y) / d.y;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((x_min <= p.x && p.x <= x_max) &&
                (z_min <= p.z && p.z <= z_max)) {
                dvec3 d2 = p - m_pos;

                PhongMaterial newMaterial = *material;
                if (texture != nullptr) {
                    newMaterial.m_kd = texture->getPixel(1 - d2.x / dims.x, d2.z / dims.z);
                }

                dvec3 normal(0, -1, 0);
                if (normalMap != nullptr) {
                    dvec3 normalOffset = normalMap->getNormalOffset(1 - d2.x / dims.x, d2.z / dims.z);
                    normal = dvec3(
                        normalOffset.x,
                        -normalOffset.z,
                        -normalOffset.y
                    );
                }

                intersections.push_back(
                    Intersection(p, normal, objCenter(), newMaterial)
                );
            }
        }
    }

    { // top face
        double t = (y_max - e.y) / d.y;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((x_min <= p.x && p.x <= x_max) &&
                (z_min <= p.z && p.z <= z_max)) {
                dvec3 d2 = p - m_pos;

                PhongMaterial newMaterial = *material;
                if (texture != nullptr) {
                    newMaterial.m_kd = texture->getPixel(d2.x / dims.x, d2.z / dims.z);
                }

                dvec3 normal(0, 1, 0);
                if (normalMap != nullptr) {
                    dvec3 normalOffset = normalMap->getNormalOffset(d2.x / dims.x, d2.z / dims.z);
                    normal = dvec3(
                        normalOffset.x,
                        normalOffset.z,
                        -normalOffset.y
                    );
                }

                intersections.push_back(
                    Intersection(p, normal, objCenter(), newMaterial)
                );
            }
        }
    }

    { // back face
        double t = (z_min - e.z) / d.z;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((x_min <= p.x && p.x <= x_max) &&
                (y_min <= p.y && p.y <= y_max)) {
                dvec3 d2 = p - m_pos;

                PhongMaterial newMaterial = *material;
                if (texture != nullptr) {
                    newMaterial.m_kd = texture->getPixel(1 - d2.x / dims.x, 1 - d2.y / dims.y);
                }

                dvec3 normal(0, 0, -1);
                if (normalMap != nullptr) {
                    dvec3 normalOffset = normalMap->getNormalOffset(1 - d2.x / dims.x, 1 - d2.y / dims.y);
                    normal = dvec3(
                        -normalOffset.x,
                        normalOffset.y,
                        -normalOffset.z
                    );
                }

                intersections.push_back(
                    Intersection(p, normal, objCenter(), newMaterial)
                );
            }
        }
    }

    { // front face
        double t = (z_max - e.z) / d.z;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((x_min <= p.x && p.x <= x_max) &&
                (y_min <= p.y && p.y <= y_max)) {
                dvec3 d2 = p - m_pos;

                PhongMaterial newMaterial = *material;
                if (texture != nullptr) {
                    newMaterial.m_kd = texture->getPixel(d2.x / dims.x, 1 - d2.y / dims.y);
                }

                dvec3 normal(0, 0, 1);
                if (normalMap != nullptr) {
                    normal = normalMap->getNormalOffset(d2.x / dims.x, 1 - d2.y / dims.y);
                }

                intersections.push_back(
                    Intersection(p, normal, objCenter(), newMaterial)
                );
            }
        }
    }

    return intersections;
}

bool Cube::isInsideTransformed(const glm::dvec3& point) const {
    return (
        m_pos.x < point.x && point.x < (m_pos.x + dims.x) &&
        m_pos.y < point.y && point.y < (m_pos.y + dims.y) &&
        m_pos.z < point.z && point.z < (m_pos.z + dims.z)
    );
}

glm::dvec3 Cube::objCenter() const {
    return m_pos + dims / 2.0;
}

AABB Cube::getAABB() const {
    return AABB(
        m_pos,
        m_pos + dims,
        getTransform()
    );
}

Cube* Cube::clone() const {
    return new Cube(*this);
}
