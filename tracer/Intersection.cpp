#include "Intersection.hpp"

#include <glm/ext.hpp>

using glm::dvec3;

Intersection::Intersection()
    : intersected(false),
      point(),
      normal(),
      objCenter(),
      material(PhongMaterial()) {}

Intersection::Intersection(
    const dvec3& point,
    const dvec3& normal,
    const dvec3& objCenter,
    const PhongMaterial& material
) : intersected(true),
    point(point),
    normal(normal),
    objCenter(objCenter),
    material(material) {}


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

LineSegment::LineSegment(const Intersection& near, const Intersection& far)
    : near(near), far(far) {
    assert(near.intersected && far.intersected && "Line segment has one end unbounded");
}

