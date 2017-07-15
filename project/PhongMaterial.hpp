#pragma once

#include <glm/glm.hpp>

struct PhongMaterial {
    PhongMaterial()
        : m_kd(), m_ks() {}

    PhongMaterial(
        const glm::dvec3& kd,
        const glm::dvec3& ks,
        double shininess,
        double glossiness,
        double reflectivity,
        double transparency,
        double refractiveIndex
    ) : m_kd(kd),
        m_ks(ks),
        m_shininess(shininess),
        glossiness(glossiness),
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

    glm::dvec3 m_ks;
    glm::dvec3 m_kd;

    double m_shininess;

    double glossiness;

    double reflectivity;

    double transparency;
    double refractiveIndex;
};
