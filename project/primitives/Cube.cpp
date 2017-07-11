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


struct BoxFace {
    dvec3 point1;
    dvec3 point2;
    dvec3 normal;
    char dim;
};

const double EPS = 0.0000001;

std::vector<LineSegment> Cube::allIntersectPostTransform(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<Intersection> intersections;

    {
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
                        Intersection(p, dvec3(-1, 0, 0), this)
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
                        Intersection(p, dvec3(1, 0, 0), this)
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
                        Intersection(p, dvec3(0, -1, 0), this)
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
                        Intersection(p, dvec3(0, 1, 0), this)
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
                        Intersection(p, dvec3(0, 0, -1), this)
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
                        Intersection(p, dvec3(0, 0, 1), this)
                    );
                }
            }
        }
    }

    std::sort(
        intersections.begin(), intersections.end(),
        [&rayOrigin](const Intersection& a, const Intersection& b) {
            return glm::distance2(rayOrigin, a.point) <
                   glm::distance2(rayOrigin, b.point);
        }
    );

    // remove 2 intersections from 1 point
    // due to floating point errors
    if (intersections.size() > 2) {
        for (size_t i = 1; i < intersections.size();) {
            double dist = glm::distance(intersections.at(i).point, intersections.at(i - 1).point);

            if (fabs(dist) < EPS) {
                intersections.erase(intersections.begin() + i);
            }
            else {
                i++;
            }
        }
    }

    if (intersections.size() == 0 || intersections.size() == 1) {
        return {};
    }
    else if (intersections.size() == 2) {
        return {
            LineSegment(intersections.at(0), intersections.at(1))
        };
    }
    else {
        cerr << glm::to_string(rayOrigin) << endl;
        cerr << glm::to_string(rayDirection) << endl;
        for (const Intersection& i: intersections) {
            cerr << glm::to_string(i.point) << endl;
        }
        assert(false && "Cube should have between 0 and 2 intersections with ray.");
    }
}

bool Cube::isInside(const glm::dvec3 point) const {
    return (
        m_pos.x < point.x && point.x < (m_pos.x + dims.x) &&
        m_pos.y < point.y && point.y < (m_pos.y + dims.y) &&
        m_pos.z < point.z && point.z < (m_pos.z + dims.z)
    );
}
