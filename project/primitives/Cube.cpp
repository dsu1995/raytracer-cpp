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
};

const double EPS = 0.0001;

Intersection2 Cube::intersect2(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<Intersection> intersections;

    dvec3 pos(m_pos);
    std::vector<BoxFace> faces = {
        {pos,                       pos + dvec3(dims.x, dims.y, 0), {0,  0,  -1}},
        {pos,                       pos + dvec3(dims.x, 0, dims.z), {0,  -1, 0}},
        {pos,                       pos + dvec3(0, dims.y, dims.z), {-1, 0,  0}},
        {pos + dvec3(dims.x, 0, 0), pos + dims,                     {1,  0,  0}},
        {pos + dvec3(0, dims.y, 0), pos + dims,                     {0,  1,  0}},
        {pos + dvec3(0, 0, dims.z), pos + dims,                     {0,  0,  1}}
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
            (face.point1.x - EPS <= intersectionPoint.x && intersectionPoint.x <= face.point2.x + EPS) &&
            (face.point1.y - EPS <= intersectionPoint.y && intersectionPoint.y <= face.point2.y + EPS) &&
            (face.point1.z - EPS <= intersectionPoint.z && intersectionPoint.z <= face.point2.z + EPS)
            ) {
            Intersection intersection(intersectionPoint, face.normal);

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
