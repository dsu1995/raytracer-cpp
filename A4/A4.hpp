#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "GeometryNode.hpp"
#include "Mesh.hpp"

class A4 {
public:
	A4(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
	);

    void render();

private:
    struct Hit {
        bool hasHit;
        glm::dvec3 point;
        glm::dvec3 normal;
        GeometryNode* node;
        double t;
    };

    void print();

    glm::dvec3 rayColour(
        uint x,
        uint y,
        const glm::dvec3& origin,
        const glm::dvec3& direction,
        const Hit& hit,
        Light* light,
        uint maxHits
    );

    bool between(
        const glm::dvec3& p1,
        const glm::dvec3& p2
    );

    Hit hit(
        const glm::dvec3& origin,
        const glm::dvec3& direction
    );

    glm::dvec3 background(
        uint x,
        uint y,
        const glm::dvec3& rayDirection
    );

    Hit rayTriangleIntersect(const glm::dvec3& p0, const glm::dvec3& p1, const glm::dvec3& p2, const glm::dvec3& origin,
                                 const glm::dvec3& direction, GeometryNode* node);

    A4::Hit meshIntersect(Mesh* mesh, const glm::dvec3 origin, const glm::dvec3 direction, GeometryNode* node);


    SceneNode* const root;
    Image& image;

    const glm::dvec3 eye;
    const glm::dvec3 view;
    const glm::dvec3 up;
    const double fovy;

    const glm::dvec3 ambient;
    const std::list<Light*>& lights;

    const size_t imageHeight;
    const size_t imageWidth;

    const glm::dvec4 rayOrigin;

    glm::dmat4 MVW;

    Hit hierHit(
        SceneNode* node,
        const glm::dvec3& origin,
        const glm::dvec3& direction
    );

    A4::Hit
    nonHierBoxIntersect(NonhierBox* box, const glm::dvec3 origin, const glm::dvec3 direction, GeometryNode* node);

    A4::Hit nonHierSphereIntersect(NonhierSphere* sphere, const glm::dvec3 origin, const glm::dvec3 direction,
                                       GeometryNode* node);

    const Hit& minHit(
        const glm::dvec3 origin,
        const Hit& h1,
        const Hit& h2
    ) const;
};
