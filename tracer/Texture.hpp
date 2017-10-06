#pragma once

#include <lodepng/lodepng.h>

#include <glm/glm.hpp>


class Texture {
public:
    Texture(
        const char* fileName,
        double xratio = 1,
        double yratio = 1
    );

    glm::dvec3 getPixel(double x, double y) const;

private:
    double xratio;
    double yratio;

    uint width;
    uint height;

    std::vector<glm::dvec3> pixels;
};



