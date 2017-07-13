#include "Cube.hpp"
#include <vector>
#include <algorithm>
#include <glm/ext.hpp>
#include <cmath>

// debug
#include <iostream>

using std::cerr;
using std::endl;

using glm::vec3;
using glm::dvec3;

Cube::Cube(const dvec3& pos, const dvec3& dims)
    : m_pos(pos), dims(dims) {}

Cube::Cube(const dvec3& pos, double size)
    : Cube(pos, dvec3(size)) {}

Cube::Cube()
    : Cube(vec3(0), 1) {}


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
                intersections.push_back(
                    Intersection(p, dvec3(-1, 0, 0), objCenter(), this)
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
                intersections.push_back(
                    Intersection(p, dvec3(1, 0, 0), objCenter(), this)
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
                intersections.push_back(
                    Intersection(p, dvec3(0, -1, 0), objCenter(), this)
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
                intersections.push_back(
                    Intersection(p, dvec3(0, 1, 0), objCenter(), this)
                );
            }
        }
    }

    { // front face
        double t = (z_min - e.z) / d.z;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((x_min <= p.x && p.x <= x_max) &&
                (y_min <= p.y && p.y <= y_max)) {
                intersections.push_back(
                    Intersection(p, dvec3(0, 0, -1), objCenter(), this)
                );
            }
        }
    }

    { // back face
        double t = (z_max - e.z) / d.z;
        if (t >= 0) {
            dvec3 p = e + t * d;
            if ((x_min <= p.x && p.x <= x_max) &&
                (y_min <= p.y && p.y <= y_max)) {
                intersections.push_back(
                    Intersection(p, dvec3(0, 0, 1), objCenter(), this)
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
    return m_pos + dims / 2;
}
