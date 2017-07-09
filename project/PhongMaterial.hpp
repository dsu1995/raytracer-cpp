#pragma once

#include <glm/glm.hpp>

class PhongMaterial {
public:
    PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);

    virtual ~PhongMaterial();

    const glm::vec3 m_kd;
    const glm::vec3 m_ks;

    const double m_shininess;
};
