#pragma once

#include "ConvexSolid.hpp"

class Cube : public ConvexSolid {
public:
    Cube(const glm::dvec3& pos, double size);

    Cube(const glm::dvec3& pos, const glm::dvec3& dims);

    Cube();

    const glm::dvec3 m_pos;
    const glm::dvec3 dims;

    glm::dvec3 objCenter() const;

    virtual AABB getAABB() const override;

    Cube* clone() const override;

protected:
    std::vector<Intersection>
    getIntersectionsPostTransform(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) const override;

    bool isInsideTransformed(const glm::dvec3& point) const override;
};


