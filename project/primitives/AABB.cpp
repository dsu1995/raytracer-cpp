#include "AABB.hpp"

#include <algorithm>

using glm::dvec3;
using glm::dvec4;
using glm::dmat4;

AABB::AABB(const dvec3& p1, const dvec3& p2)
    : p1(p1), p2(p2) {}

AABB::AABB(const glm::dvec3& p1, const glm::dvec3& p2, const glm::dmat4 T)
: AABB(p1, p2) {
    transform(T);
}

void AABB::transform(const dmat4& T) {
    dvec3 p3(p1.x, p1.y, p2.z);
    dvec3 p4(p1.x, p2.y, p1.z);
    dvec3 p5(p2.x, p1.y, p1.z);

    dvec3 p6(p2.x, p2.y, p1.z);
    dvec3 p7(p2.x, p1.y, p2.z);
    dvec3 p8(p1.x, p2.y, p2.z);

    dvec3 newP1(std::numeric_limits<double>::max());
    dvec3 newP2(std::numeric_limits<double>::min());

    for (const dvec3 p: {p1, p2, p3, p4, p5, p6, p7, p8}) {
        dvec3 transformedP(T * dvec4(p, 1));

        for (uint i = 0; i < 3; i++) {
            newP1[i] = std::min(newP1[i], transformedP[i]);
            newP2[i] = std::max(newP2[i], transformedP[i]);
        }
    }

    p1 = newP1;
    p2 = newP2;
}

bool AABB::intersects(const AABB& other) const {
    if (other.p2.x < this->p1.x) return false;
    if (this->p2.x < other.p1.x) return false;

    if (other.p2.y < this->p1.y) return false;
    if (this->p2.y < other.p1.y) return false;

    if (other.p2.z < this->p1.z) return false;
    if (this->p2.z < other.p1.z) return false;

    return true;
}
