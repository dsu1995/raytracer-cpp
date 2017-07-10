#include "Intersection.hpp"

#include <glm/ext.hpp>

using glm::dvec3;

Intersection::Intersection()
    : intersected(false),
      point(),
      normal(),
      primitive(nullptr) {}

Intersection::Intersection(
    const dvec3& point,
    const dvec3& normal,
    const Primitive* node
) : intersected(true),
    point(point),
    normal(normal),
    primitive(node) {}


const Intersection& Intersection::min(
    const dvec3& rayOrigin,
    const Intersection& i1,
    const Intersection& i2
) {
    if (!i1.intersected && !i2.intersected) {
        return i1;
    }
    else if (i1.intersected && !i2.intersected) {
        return i1;
    }
    else if (!i1.intersected && i2.intersected) {
        return i2;
    }
    else {
        if (glm::distance2(rayOrigin, i1.point) < glm::distance2(rayOrigin, i2.point)) {
            return i1;
        }
        else {
            return i2;
        }
    }
}

LineSegment::LineSegment()
    : near(), far() {}

LineSegment::LineSegment(const Intersection& near, const Intersection& far)
    : near(near), far(far) {
    assert(near.intersected && far.intersected && "Line segment has one end unbounded");
}


bool LineSegment::isEmpty() const {
    return !near.intersected;
}
