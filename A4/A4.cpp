#include <glm/ext.hpp>
#include <cmath>

#include "A4.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;

const float NEAR_PLANE_DISTANCE = 1;

void A4_Render(
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
) {
    // Fill in raytracing code here...

    std::cout << "Calling A4_Render(\n" <<
          "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
          "\t" << "view: " << glm::to_string(view) << std::endl <<
          "\t" << "up:   " << glm::to_string(up) << std::endl <<
          "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
          "\t" << "lights{" << std::endl;

    for (const Light * light : lights) {
        std::cout << "\t\t" <<  *light << std::endl;
    }
    std::cout << "\t}" << std::endl;
    std::cout <<")" << std::endl;
    // end printing


    size_t ny = image.height();
    size_t nx = image.width();

    // Step 1
    const mat4 T1 = glm::translate(vec3(-nx / 2.0, ny / 2.0, NEAR_PLANE_DISTANCE));
    // Step 2
    const float h = 2 * NEAR_PLANE_DISTANCE * tanf(float(fovy) / 2);
    const mat4 S2 = glm::scale(vec3(-h / ny, h / ny, 1));
    // Step 3
    const vec3 w = glm::normalize(view - eye);
    const vec3 u = glm::normalize(glm::cross(up, w));
    const vec3 v = glm::cross(w, u);
    const mat4 R3(
        vec4(u, 0),
        vec4(v, 0),
        vec4(w, 0),
        vec4(0, 0, 0, 1)
    );
    // Step 4
    const mat4 T4 = glm::translate(eye);
    const mat4 MVW = T4 * R3 * S2 * T1;

    for (uint y = 0; y < ny; y++) {
        for (uint x = 0; x < nx; x++) {
            const vec4 screenCoordPixel(x, y, 0, 1);
            const vec4 worldCoordPixel = MVW * screenCoordPixel;

            const vec4 rayOrigin(eye, 1);
            const vec4 rayDirection = worldCoordPixel - rayOrigin;




//            // Red: increasing from top to bottom
//            image(x, y, 0) = double(y) / ny;
//            // Green: increasing from left to right
//            image(x, y, 1) = double(x) / nx;
//            // Blue: in lower-left and upper-right corners
//            image(x, y, 2) = ((y < ny/2 && x < nx/2)
//                          || (y >= ny/2 && x >= nx/2)) ? 1.0 : 0.0;
        }
    }
}
