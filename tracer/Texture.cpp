#include "Texture.hpp"

#include <iostream>

using std::cerr;
using std::endl;
using glm::dvec3;

Texture::Texture(
    const char* fileName,
    double xratio,
    double yratio
) : xratio(xratio),
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
            image.at(i * 4 + 0) / 256.0,
            image.at(i * 4 + 1) / 256.0,
            image.at(i * 4 + 2) / 256.0
        ));
    }
}


dvec3 Texture::getPixel(double x, double y) const {
    assert(0.0 <= x && x <= 1.0 && 0.0 <= y && y <= 1.0);

    x = x * xratio - floor(x * xratio);
    y = y * yratio - floor(y * yratio);

    uint xfloor = uint(floor(x * (width - 1)));
    uint xceil = uint(ceil(x * (width - 1)));

    uint yfloor = uint(floor(y * (height - 1)));
    uint yceil = uint(ceil(y * (height - 1)));

    dvec3 p00 = pixels.at(yfloor * width + xfloor);
    dvec3 p01 = pixels.at(yceil * width + xfloor);
    dvec3 p10 = pixels.at(yfloor * width + xceil);
    dvec3 p11 = pixels.at(yceil * width + xceil);

    dvec3 bottom = p00 + (p10 - p00) * x;
    dvec3 top = p01 + (p11 - p01) * x;

    // TODO it seems bilinear interpolation makes the image look worse
//    return bottom + (top - bottom) * y;
    return p00;
}
