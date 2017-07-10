#include "Transformable.hpp"

#include <glm/ext.hpp>

using glm::dvec3;
using glm::dmat4;

Transformable::Transformable()
    : transform(),
      invTransform() {}

const dmat4& Transformable::getTransform() const {
    return transform;
}

const dmat4& Transformable::getInvTransform() const {
    return invTransform;
}

void Transformable::rotate(char axis, double angle) {
    dvec3 rotAxis;

    switch (axis) {
        case 'x':
            rotAxis = dvec3(1, 0, 0);
            break;
        case 'y':
            rotAxis = dvec3(0, 1, 0);
            break;
        case 'z':
            rotAxis = dvec3(0, 0, 1);
            break;
        default:
            assert(false && "Invalid rotation axis!");
    }

    dmat4 rotMatrix = glm::rotate(glm::radians(angle), rotAxis);
    transform = rotMatrix * transform;

    dmat4 invRotMatrix = glm::rotate(glm::radians(-angle), rotAxis);
    invTransform *= invRotMatrix;
}

void Transformable::scale(const dvec3& amount) {
    dmat4 scaleMatrix = glm::scale(amount);
    transform = scaleMatrix * transform;

    dmat4 invScaleMatrix = glm::scale(
        dvec3(
            1 / amount.x,
            1 / amount.y,
            1 / amount.z
        )
    );
    invTransform *= invScaleMatrix;
}

void Transformable::translate(const dvec3& amount) {
    dmat4 translationMatrix = glm::translate(amount);
    transform = translationMatrix * transform;

    dmat4 invTranslationMatrix = glm::translate(-amount);
    invTransform *= invTranslationMatrix;
}




