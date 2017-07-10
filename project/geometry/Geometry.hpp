#pragma once

#include <glm/glm.hpp>
#include "../Intersection.hpp"
#include "../Transformable.hpp"
#include "../PhongMaterial.hpp"


class Geometry : public Transformable {
public:
    Geometry();

    virtual ~Geometry() {}

    virtual Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;

    PhongMaterial* getMaterial() const;

    void setMaterial(PhongMaterial* material);

private:
    PhongMaterial* material;
};
