#pragma once

#include <lodepng/lodepng.h>

#include <glm/glm.hpp>


class NormalMap {
public:
    NormalMap(
        const char* fileName,
        double exaggeration = 1,
        double xratio = 1,
        double yratio = 1
    );

    glm::dvec3 getNormalOffset(double x, double y) const;

private:
    double exaggeration;
    double xratio;
    double yratio;

    uint width;
    uint height;

    std::vector<glm::dvec3> pixels;
};



