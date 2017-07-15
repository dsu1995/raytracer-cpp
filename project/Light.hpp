#pragma once

#include <iosfwd>

#include <glm/glm.hpp>

// Represents a simple point light.
struct Light {
    Light(
        const glm::dvec3& colour,
        const glm::dvec3& position,
        const glm::dvec3& falloff,
        double radius
    ) : colour(colour),
        position(position),
        falloff(falloff),
        radius(radius) {}

    glm::dvec3 colour;
    glm::dvec3 position;
    glm::dvec3 falloff;

    double radius;
};

std::ostream& operator<<(std::ostream& out, const Light& l);