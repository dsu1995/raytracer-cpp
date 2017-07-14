#include "Primitive.hpp"

Primitive::Primitive()
    : material(nullptr),
      texture(nullptr) {}

Primitive::~Primitive() {}

void Primitive::setMaterial(PhongMaterial* material) {
    // Obviously, there's a potential memory leak here.  A good solution
    // would be to use some kind of reference counting, as in the
    // C++ shared_ptr.  But I'm going to punt on that problem here.
    // Why?  Two reasons:
    // (a) In practice we expect the scene to be constructed exactly
    //     once.  There's no reason to believe that materials will be
    //     repeatedly overwritten in a GeometryNode.
    // (b) A ray tracer is a program in which you compute once, and
    //     throw away all your data.  A memory leak won't build up and
    //     crash the program.

    this->material = material;
}



bool Primitive::isInside(const glm::dvec3& point) const {
    Ray ray = transformRay(point, glm::dvec3());
    const glm::dvec3& newPoint = ray.rayOrigin;

    return isInsideTransformed(newPoint);
}

void Primitive::setTexture(Texture* texture) {
    this->texture = texture;
}
