#pragma once

#include "ConvexSolid.hpp"

class Cube : public ConvexSolid {
public:
    Cube(const glm::dvec3& pos, double size);

    Cube(const glm::dvec3& pos, const glm::dvec3& dims);

    Cube();

    const glm::vec3 m_pos;
    const glm::dvec3 dims;

protected:
    std::vector<Intersection>
    getIntersectionsPostTransform(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) const override;

    bool isInsideTransformed(const glm::dvec3& point) const override;
};


