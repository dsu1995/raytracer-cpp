#include "Cube.hpp"
#include <vector>

using glm::vec3;
using glm::dvec3;

Cube::~Cube() {}

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

Intersection Cube::intersect(
    const dvec3& rayOrigin,
    const dvec3& rayDirection
) const {
    Intersection closest;

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

            closest = Intersection::min(rayOrigin, closest, intersection);
        }
    }

    return closest;
}
