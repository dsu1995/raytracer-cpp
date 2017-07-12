#pragma once

#include <glm/glm.hpp>

class PhongMaterial {
public:
    PhongMaterial(
        const glm::dvec3& kd,
        const glm::dvec3& ks,
        double shininess,
        double reflectivity,
        double transparency,
        double refractiveIndex
    );

    virtual ~PhongMaterial();

    const glm::dvec3 m_kd;
    const glm::dvec3 m_ks;

    const double m_shininess;
    const double reflectivity;
    const double transparency;
    const double refractiveIndex;
};
