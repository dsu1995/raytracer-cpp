#include "Transformable.hpp"

#include <glm/ext.hpp>

using glm::dvec3;
using glm::dvec4;
using glm::dmat3;
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


Ray Transformable::transformRay(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    dvec3 newOrigin(invTransform * dvec4(rayOrigin, 1));
    dvec3 newDirection(invTransform * dvec4(rayDirection, 0));
    return {newOrigin, newDirection};
}

Intersection Transformable::transformIntersectionBack(
    Intersection intersection
) const {
    if (intersection.intersected) {
        intersection.point = dvec3(dmat4(transform) * glm::vec4(intersection.point, 1));

        dmat3 normalTransform(invTransform);
        normalTransform = glm::transpose(normalTransform);
        intersection.normal = normalTransform * intersection.normal;
    }
    return intersection;
}

void Transformable::setTransform(const glm::dmat4& trans, const glm::dmat4& inv) {
    transform = trans;
    invTransform = inv;
}


