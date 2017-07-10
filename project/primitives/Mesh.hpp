#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"
#include "Cube.hpp"

struct Triangle {
    size_t v1;
    size_t v2;
    size_t v3;

    Triangle(size_t pv1, size_t pv2, size_t pv3)
        : v1(pv1),
          v2(pv2),
          v3(pv3) {}
};

// A polygonal mesh.
class Mesh : public Primitive {
public:
    Mesh(const std::string& fname);

    const std::vector<glm::vec3>& vertices() const;

    const std::vector<Triangle>& faces() const;

    virtual Intersection closestIntersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override {
        assert(false && "Mesh cannot be used in CSG!");
    }

private:
    std::vector<glm::vec3> m_vertices;
    std::vector<Triangle> m_faces;

    glm::vec3 point1;
    glm::vec3 point2;

    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);

    Intersection rayTriangleIntersect(
        const glm::dvec3& p0,
        const glm::dvec3& p1,
        const glm::dvec3& p2,
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const;
};
