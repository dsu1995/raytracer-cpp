#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

using glm::dvec3;
using glm::dmat3;

PhongMaterial dummyMaterial;


Intersection Mesh::rayTriangleIntersect(
    const dvec3& p0,
    const dvec3& p1,
    const dvec3& p2,
    const dvec3& rayOrigin,
    const dvec3& rayDirection
) const {
    dvec3 R = rayOrigin - p0;

    dmat3 M(
        p1 - p0,
        p2 - p0,
        -rayDirection
    );

    dvec3 solution = glm::inverse(M) * R;
    double beta = solution.x;
    double gamma = solution.y;
    double t = solution.z;

    if (beta >= 0 && gamma >= 0 && beta + gamma <= 1 && t >= 0) {
        dvec3 intersection = rayOrigin + t * rayDirection;
        dvec3 normal = glm::cross(p1 - p0, p2 - p1); // TODO check normal direction
        return {intersection, normal, boundingBox->objCenter(), *material};
    }
    else {
        return {};
    }
}

Mesh::Mesh(const std::string& fname)
    : m_vertices(),
      m_faces() {
    std::string code;
    double vx, vy, vz;
    size_t s1, s2, s3;

    std::ifstream ifs(fname.c_str());
    while (ifs >> code) {
        if (code == "v") {
            ifs >> vx >> vy >> vz;
            m_vertices.push_back(glm::dvec3(vx, vy, vz));
        }
        else if (code == "f") {
            ifs >> s1 >> s2 >> s3;
            m_faces.push_back(Triangle(s1 - 1, s2 - 1, s3 - 1));
        }
    }

    glm::dvec3 point1(std::numeric_limits<float>::max());
    glm::dvec3 point2(std::numeric_limits<float>::min());

    for (const glm::dvec3& vertex: m_vertices) {
        for (uint i = 0; i < 3; i++) {
            point1[i] = std::min(point1[i], vertex[i]);
            point2[i] = std::max(point2[i], vertex[i]);
        }
    }

    boundingBox = new Cube(point1, point2 - point1);
    boundingBox->setMaterial(&dummyMaterial);
}

const std::vector<Triangle>& Mesh::faces() const {
    return m_faces;
}

const std::vector<glm::dvec3>& Mesh::vertices() const {
    return m_vertices;
}

Intersection Mesh::getClosestIntersection(
    const dvec3& oldOrigin,
    const dvec3& oldDirection
) const {
    Ray transformedRay = transformRay(oldOrigin, oldDirection);
    const glm::dvec3& rayOrigin = transformedRay.rayOrigin;
    const glm::dvec3& rayDirection = transformedRay.rayDirection;

    if (!boundingBox->getClosestIntersection(rayOrigin, rayDirection).intersected) {
        return {};
    }

    Intersection closest;
    for (const Triangle& triangle: m_faces) {
        Intersection triangleIntersection = rayTriangleIntersect(
            m_vertices.at(triangle.v1),
            m_vertices.at(triangle.v2),
            m_vertices.at(triangle.v3),
            rayOrigin,
            rayDirection
        );

        closest = Intersection::min(rayOrigin, closest, triangleIntersection);
    }

    return transformIntersectionBack(closest);
}

Mesh::~Mesh() {
    delete boundingBox;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh) {
    out << "mesh {";
    /*

    for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
        const MeshVertex& v = mesh.m_verts[idx];
        out << glm::to_string( v.m_position );
      if( mesh.m_have_norm ) {
          out << " / " << glm::to_string( v.m_normal );
      }
      if( mesh.m_have_uv ) {
          out << " / " << glm::to_string( v.m_uv );
      }
    }

  */
    out << "}";
    return out;
}

