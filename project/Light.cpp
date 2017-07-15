#include "Light.hpp"

#include <iostream>

#include <glm/ext.hpp>

std::ostream& operator<<(std::ostream& out, const Light& l) {
    out << "L["
        << glm::to_string(l.colour) << ", "
        << glm::to_string(l.position) << ", "
        << glm::to_string(l.falloff)
        << "]";
    return out;
}