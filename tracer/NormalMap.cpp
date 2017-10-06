#include "NormalMap.hpp"

#include <iostream>

using std::cerr;
using std::endl;

NormalMap::NormalMap(
    const char* fileName,
    double exaggeration,
    double xratio,
    double yratio
) : exaggeration(exaggeration),
    xratio(xratio),
    yratio(yratio) {

    std::vector<unsigned char> image; //the raw pixels

    uint error = lodepng::decode(image, width, height, fileName);
    //if there's an error, display it
    if (error) {
        cerr << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    }
    assert(width * height * 4 == image.size());

    for (size_t i = 0; i < width * height; i++) {
        pixels.push_back(glm::dvec3(
            image.at(i * 4 + 0) / 128.0 - 1,
            image.at(i * 4 + 1) / 128.0 - 1,
            image.at(i * 4 + 2) / 128.0 - 1
        ));
    }
}

glm::dvec3 NormalMap::getNormalOffset(double x, double y) const {
    assert(0.0 <= x && x <= 1.0 && 0.0 <= y && y <= 1.0);

    x = x * xratio - floor(x * xratio);
    y = y * yratio - floor(y * yratio);

    uint x2 = uint(x * (width - 1));
    uint y2 = uint(y * (height - 1));

    uint offset = y2 * width + x2;

    glm::dvec3 normalOffset = pixels.at(offset);
    normalOffset.z /= exaggeration;

    return normalOffset;
}
