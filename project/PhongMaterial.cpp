#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
    const glm::dvec3& kd,
    const glm::dvec3& ks,
    double shininess,
    double reflectivity
) : m_kd(kd),
    m_ks(ks),
    m_shininess(shininess),
    reflectivity(reflectivity) {

    assert(
        0.0 <= reflectivity && reflectivity <= 1.0 &&
            "Reflectivity must be between 0 and 1!"
    );
}

PhongMaterial::~PhongMaterial() {}
