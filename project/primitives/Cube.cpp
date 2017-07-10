#include "Cube.hpp"
#include <vector>
#include <algorithm>
#include <glm/ext.hpp>

using glm::vec3;
using glm::dvec3;

Cube::Cube(const vec3& pos, const vec3& dims)
    : m_pos(pos), dims(dims) {}

Cube::Cube(const vec3& pos, double size)
    : Cube(pos, vec3(float(size))) {}

Cube::Cube()
    : Cube(vec3(0), 1) {}


struct BoxFace {
    dvec3 point1;
    dvec3 point2;
    dvec3 normal;
    char dim;
};

const double EPS = 0.0001;

std::vector<LineSegment> Cube::allIntersectPostTransform(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<Intersection> intersections;

    dvec3 pos(m_pos);
    std::vector<BoxFace> faces = {
        {pos,                       pos + dvec3(dims.x, dims.y, 0), {0,  0,  -1}, 'z'},
        {pos,                       pos + dvec3(dims.x, 0, dims.z), {0,  -1, 0}, 'y'},
        {pos,                       pos + dvec3(0, dims.y, dims.z), {-1, 0,  0}, 'x'},
        {pos + dvec3(dims.x, 0, 0), pos + dims,                     {1,  0,  0}, 'x'},
        {pos + dvec3(0, dims.y, 0), pos + dims,                     {0,  1,  0}, 'y'},
        {pos + dvec3(0, 0, dims.z), pos + dims,                     {0,  0,  1}, 'z'}
    };

    for (const BoxFace& face: faces) {
        // Backface culling breaks when the ray starts inside the cube
//        if (glm::dot(face.normal, direction) >= 0) {
//            continue;
//        }
        double wecA = glm::dot(rayOrigin - face.point1, face.normal);
        double wecB = glm::dot(rayOrigin + rayDirection - face.point1, face.normal);
        double t = wecA / (wecA - wecB);
        if (t < 0) {
            continue;
        }
        dvec3 intersectionPoint = rayOrigin + t * rayDirection;

        if (
            face.dim != 'x' &&
            !(face.point1.x <= intersectionPoint.x && intersectionPoint.x <= face.point2.x)
        ) {
            continue;
        }

        if (
            face.dim != 'y' &&
            !(face.point1.y <= intersectionPoint.y && intersectionPoint.y <= face.point2.y)
        ) {
            continue;
        }

        if (
            face.dim != 'z' &&
            !(face.point1.z <= intersectionPoint.z && intersectionPoint.z <= face.point2.z)
        ) {
            continue;
        }

        Intersection intersection(intersectionPoint, face.normal, this);
        intersections.push_back(intersection);
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
