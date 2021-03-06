#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "nodes/SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "nodes/GeometryNode.hpp"
#include "primitives/Mesh.hpp"

#include "Scene.hpp"

class Project {
public:
    Project(
        // What to render
        SceneNode* root,

        // Image to write to, set to a given width and height
        Image& image,

        // Viewing parameters
        const glm::dvec3& eye,
        const glm::dvec3& view,
        const glm::dvec3& up,
        double fovy,

        // Lighting parameters
        const glm::dvec3& ambient,
        const std::list<Light*>& lights,
        bool supersample,
        uint distributedSamples,
        bool gridAcceleration
    );

    void render();

private:
    glm::dvec3 renderPixel(double x, double y) const;

    glm::dvec3 traceRecursive(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection,
        uint recursionDepth,
        uint distributedSamples
    ) const;

    glm::dvec3 refractRecursive(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection,
        uint recursionDepth,
        uint distributedSamples
    ) const;

    glm::dvec3 rayColour(
        const glm::dvec3& origin,
        const glm::dvec3& direction,
        const Intersection& intersection,
        Light* light
    ) const;

    glm::dvec3 background(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const;

    void print() const;


    SceneNode* const root;
    Scene scene;

    Image& image;

    const glm::dvec3& eye;
    const glm::dvec3& view;
    const glm::dvec3& up;

    const double fovy;
    const glm::dvec3& ambient;

    const std::list<Light*>& lights;
    const bool supersample;

    const uint initDistributedSamples;

    const bool gridAcceleration;

    const size_t imageWidth;
    const size_t imageHeight;

    glm::dmat4 MVW;
};
