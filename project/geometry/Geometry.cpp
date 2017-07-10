#include "Geometry.hpp"

Geometry::Geometry()
    : material(nullptr) {}

PhongMaterial* Geometry::getMaterial() const {
    return material;
}

void Geometry::setMaterial(PhongMaterial* material) {
    this->material = material;
}
