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

const bool SUPERSAMPLE_ON = false;

Project::Project(
    // What to render
    SceneNode* root,

    // Image to write to, set to a given width and height
    Image& image,

    // Viewing parameters
    const glm::vec3& eye,
    const glm::vec3& view,
    const glm::vec3& up,
    double fovy,

    // Lighting parameters
    const glm::vec3& ambient,
    const std::list<Light*>& lights
) : root(root),
    scene(root),
    image(image),
    eye(eye),
    view(view),
    up(up),
    fovy(fovy),
    ambient(ambient),
    lights(lights),
    imageWidth(image.width()),
    imageHeight(image.height()),
    rayOrigin(eye, 1)
{
    // Step 1
    const dmat4 T1 = glm::translate(dvec3(imageWidth / -2.0, imageHeight / -2.0, NEAR_PLANE_DISTANCE));
    // Step 2
    const double h = 2 * NEAR_PLANE_DISTANCE * tan(glm::radians(fovy) / 2);
    const dmat4 S2 = glm::scale(dvec3(-h / imageHeight, -h / imageHeight, 1));
    // Step 3
    const dvec3 w = glm::normalize(dvec3(view));
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

            if (SUPERSAMPLE_ON) {
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
    const dvec4 rayDirection = worldCoordPixel - rayOrigin;

    Intersection intersection = scene.trace(dvec3(rayOrigin), dvec3(rayDirection));

    dvec3 colour;
    if (!intersection.intersected) {
        colour = background(x, y, dvec3(rayDirection));
    }
    else {
        PhongMaterial* material = intersection.primitive->getMaterial();
        dvec3 intersectionEps = intersection.point + glm::normalize(intersection.normal) * EPS;

        colour = ambient * dvec3(material->m_kd);
        for (Light* light: lights) {
            if (!scene.existsObjectBetween(intersectionEps, light->position)) {
                colour += rayColour(dvec3(rayOrigin), dvec3(rayDirection), intersection, light);
            }
        }
    }
    return colour;
}

dvec3 Project::rayColour(
    const dvec3& origin,
    const dvec3& direction,
    const Intersection& intersection,
    Light* light
) const {
    PhongMaterial* material = intersection.primitive->getMaterial();

    dvec3 k_d = material->m_kd;
    dvec3 l = glm::normalize(dvec3(light->position) - intersection.point);
    dvec3 n = glm::normalize(dvec3(intersection.normal));
    dvec3 k_s(material->m_ks);
    dvec3 r = glm::normalize(-l + 2 * glm::dot(l, n) * n);
    dvec3 v = glm::normalize(-direction);
    double p = material->m_shininess;
    dvec3 I = light->colour;
    double dist = glm::distance(dvec3(light->position), intersection.point);

    dvec3 L_in = I / (light->falloff[0] + light->falloff[1] * dist + light->falloff[2] * dist * dist);
    dvec3 L_out = (
        k_d * std::max(0.0, glm::dot(l, n)) + k_s * pow(std::max(0.0, glm::dot(r, v)), p)
    ) * L_in;

    return L_out;
}


glm::dvec3 Project::background(
    double x,
    double y,
    const glm::dvec3& rayDirection
) const {
    x /= imageWidth;
    y /= imageHeight;
    return {x, x * y, y};
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