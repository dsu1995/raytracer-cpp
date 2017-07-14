#pragma once

#include "ConvexSolid.hpp"

class Sphere : public ConvexSolid {
public:
    Sphere();

    Sphere(const glm::dvec3& pos, double radius);

    const glm::dvec3 m_pos;
    const double m_radius;

protected:
    std::vector<Intersection>
    getIntersectionsPostTransform(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) const override;

    bool isInsideTransformed(const glm::dvec3& point) const override;

private:
    PhongMaterial getMaterial(const glm::dvec3& point) const;

    glm::dvec3 getNormal(const glm::dvec3& point) const;
};

