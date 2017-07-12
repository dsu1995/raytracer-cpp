#include <glm/ext.hpp>
#include <cmath>
#include <atomic>
#include <sstream>

#include "Project.hpp"

using glm::dvec3;
using glm::dvec4;
using glm::dmat3;
using glm::dmat4;

using std::cout;
using std::endl;

const double NEAR_PLANE_DISTANCE = 1;

const double EPS = 0.0001;

const uint RECURSION_DEPTH = 3;

Project::Project(
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
    bool supersample
) : root(root),
    scene(root),
    image(image),
    eye(eye),
    view(view),
    up(up),
    fovy(fovy),
    ambient(ambient),
    lights(lights),
    supersample(supersample),
    imageWidth(image.width()),
    imageHeight(image.height())
{
    // Step 1
    const dmat4 T1 = glm::translate(dvec3(imageWidth / -2.0, imageHeight / -2.0, NEAR_PLANE_DISTANCE));
    // Step 2
    const double h = 2 * NEAR_PLANE_DISTANCE * tan(glm::radians(fovy) / 2);
    const dmat4 S2 = glm::scale(dvec3(-h / imageHeight, -h / imageHeight, 1));
    // Step 3
    const dvec3 w = glm::normalize(view);
    const dvec3 u = glm::normalize(glm::cross(this->up, w));
    const dvec3 v = glm::cross(w, u);
    const dmat4 R3(
        dvec4(u, 0),
        dvec4(v, 0),
        dvec4(w, 0),
        dvec4(0, 0, 0, 1)
    );
    // Step 4
    const dmat4 T4 = glm::translate(eye);
    MVW = T4 * R3 * S2 * T1;

    print();
}

const double INCREMENT = 0.01;

void Project::render() {
    size_t total = imageWidth * imageHeight;
    std::atomic<size_t> complete(0);
    double nextGoal = INCREMENT;

    #pragma omp parallel for
    for (uint y = 0; y < imageHeight; y++) {
        for (uint x = 0; x < imageWidth; x++) {
            dvec3 colour(0, 0, 0);

            if (supersample) {
                // super sample 9 times per pixel
                double delta = 1.0 / 3;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        colour += renderPixel(x + i * delta, y + j * delta);
                    }
                }
                colour /= 9;
            }
            else {
                colour = renderPixel(x, y);
            }

            image(x, y, 0) = colour[0];
            image(x, y, 1) = colour[1];
            image(x, y, 2) = colour[2];
        }

        complete += imageWidth;
        if (complete > total * nextGoal) {
            nextGoal += INCREMENT;
            std::stringstream sstream;
            sstream << "Pixels rendered: " << complete << '/' << total << '\r';
            cout << sstream.str() << std::flush;
        }
    }
    cout << "Pixels rendered: " << complete << '/' << total << endl;
}

dvec3 Project::renderPixel(double x, double y) const {
    const dvec4 screenCoordPixel(x, y, 0, 1);
    const dvec4 worldCoordPixel = MVW * screenCoordPixel;
    const dvec3 rayDirection = dvec3(worldCoordPixel) - eye;

    return traceRecursive(eye, rayDirection, RECURSION_DEPTH);
}

glm::dvec3 Project::traceRecursive(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection,
    uint recursionDepth
) const {
    Intersection intersection = scene.trace(rayOrigin, rayDirection);

    if (!intersection.intersected) {
        return background(rayOrigin, rayDirection);
    }
    else {
        PhongMaterial* material = intersection.primitive->getMaterial();
        dvec3 normal = glm::normalize(intersection.normal);

        // TODO glm::length(intersection.point) is a heuristic, might not work
        dvec3 intersectionEps =
            intersection.point + normal * glm::length(intersection.point) * EPS;

        double reflectivity = (recursionDepth == 1) ? 0 : material->reflectivity;

        dvec3 colour(0, 0, 0);
        if (reflectivity > 0) {
            dvec3 reflectedRay = glm::reflect(rayDirection, normal);
            dvec3 reflectedColour = traceRecursive(intersectionEps, reflectedRay, recursionDepth - 1);
            colour += reflectedColour * reflectivity;
        }

        if (reflectivity < 1) {
            dvec3 ownColour = ambient * material->m_kd;
            for (Light* light: lights) {
                if (!scene.existsObjectBetween(intersectionEps, light->position)) {
                    ownColour += rayColour(rayOrigin, rayDirection, intersection, light);
                }
            }

            colour += ownColour * (1 - reflectivity);
        }

        return colour;
    }
}


dvec3 Project::rayColour(
    const dvec3& origin,
    const dvec3& direction,
    const Intersection& intersection,
    Light* light
) const {
    PhongMaterial* material = intersection.primitive->getMaterial();

    const dvec3& k_d = material->m_kd;
    dvec3 l = glm::normalize(light->position - intersection.point);
    dvec3 n = glm::normalize(intersection.normal);
    const dvec3& k_s = material->m_ks;
    dvec3 r = glm::normalize(-l + 2 * glm::dot(l, n) * n);
    dvec3 v = glm::normalize(-direction);
    double p = material->m_shininess;
    const dvec3& I = light->colour;
    double dist = glm::distance(light->position, intersection.point);

    dvec3 L_in = I / (light->falloff[0] + light->falloff[1] * dist + light->falloff[2] * dist * dist);
    dvec3 L_out = (
        k_d * std::max(0.0, glm::dot(l, n)) + k_s * pow(std::max(0.0, glm::dot(r, v)), p)
    ) * L_in;

    return L_out;
}

glm::dvec3 Project::background(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    return dvec3(0, 0, 1);
//    return glm::normalize(glm::normalize(rayOrigin) + glm::normalize(rayDirection));
}

void Project::print() const {
    std::cout << "Calling A4_Render(\n" <<
              "\t" << *root <<
              "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
                  "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
              "\t" << "view: " << glm::to_string(view) << std::endl <<
              "\t" << "up:   " << glm::to_string(up) << std::endl <<
              "\t" << "fovy: " << fovy << std::endl <<
              "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
              "\t" << "lights{" << std::endl;

    for (const Light* light : lights) {
        std::cout << "\t\t" <<  *light << std::endl;
    }
    std::cout << "\t}" << std::endl;
    std::cout <<")" << std::endl;
}
