#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
    const glm::dvec3& kd,
    const glm::dvec3& ks,
    double shininess,
    double reflectivity,
    double transparency,
    double refractiveIndex
) : m_kd(kd),
    m_ks(ks),
    m_shininess(shininess),
    reflectivity(reflectivity),
    transparency(transparency),
    refractiveIndex(refractiveIndex) {

    assert(
        0.0 <= reflectivity && reflectivity <= 1.0 &&
            "Reflectivity must be between 0 and 1!"
    );

    assert(
        0.0 <= transparency && transparency <= 1.0 &&
        "Transparency must be between 0 and 1!"
    );
}

PhongMaterial::~PhongMaterial() {}
