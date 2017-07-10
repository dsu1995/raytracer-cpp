#pragma once

#include <glm/glm.hpp>
#include "Intersection.hpp"

struct Ray {
    glm::dvec3 rayOrigin;
    glm::dvec3 rayDirection;
};

class Transformable {
public:
    Transformable();

    virtual ~Transformable() {}

    const glm::dmat4& getTransform() const;

    const glm::dmat4& getInvTransform() const;

    void setTransform(const glm::dmat4& trans, const glm::dmat4& inv);

    void rotate(char axis, double angle);

    void scale(const glm::dvec3& amount);

    void translate(const glm::dvec3& amount);

    Ray transformRay(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) const;

    Intersection transformIntersectionBack(Intersection intersection) const;

private:
    glm::dmat4 transform;
    glm::dmat4 invTransform;
};
