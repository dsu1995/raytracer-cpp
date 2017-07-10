#pragma once

#include <glm/glm.hpp>


class Transformable {
public:
    Transformable();

    virtual ~Transformable() {}

    const glm::dmat4& getTransform() const;

    const glm::dmat4& getInvTransform() const;

    void rotate(char axis, double angle);

    void scale(const glm::dvec3& amount);

    void translate(const glm::dvec3& amount);

private:
    glm::dmat4 transform;
    glm::dmat4 invTransform;
};



