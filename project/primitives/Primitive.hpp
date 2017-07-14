#pragma once

#include <vector>

#include <glm/glm.hpp>
#include "../Intersection.hpp"
#include "../PhongMaterial.hpp"
#include "../Transformable.hpp"

class Primitive : public Transformable {
public:
    Primitive();

    virtual ~Primitive();

    void setMaterial(PhongMaterial* material);

    void setTexture(Texture* texture);

    virtual Intersection getClosestIntersection(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;

    virtual std::vector<LineSegment> getCSGSegments(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;

    virtual bool isInside(const glm::dvec3& point) const;

    PhongMaterial* material;

protected:
    virtual bool isInsideTransformed(const glm::dvec3& point) const = 0;

    Texture* texture;
};
