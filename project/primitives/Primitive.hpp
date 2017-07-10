#pragma once

#include <vector>

#include <glm/glm.hpp>
#include "../Intersection.hpp"
#include "../PhongMaterial.hpp"
#include "../Transformable.hpp"

class Primitive : public Transformable {
public:
    Primitive()
        : material(nullptr) {}

    virtual ~Primitive() {}

    virtual Intersection closestIntersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const {
        std::vector<LineSegment> segments = allIntersect(rayOrigin, rayDirection);
        if (segments.empty()) {
            return {};
        }
        else {
            return segments.front().near;
        }
    }

    std::vector<LineSegment> allIntersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const {
        Ray transformedRay = transformRay(rayOrigin, rayDirection);
        const glm::dvec3& newOrigin = transformedRay.rayOrigin;
        const glm::dvec3& newDirection = transformedRay.rayDirection;

        std::vector<LineSegment> segments = allIntersectPostTransform(newOrigin, newDirection);

        std::vector<LineSegment> transformedSegments;
        for (const LineSegment& segment: segments) {
            const Intersection& near = segment.near;
            const Intersection& far = segment.far;

            transformedSegments.push_back(
                LineSegment(
                    transformIntersectionBack(near),
                    transformIntersectionBack(far)
                )
            );
        }

        return transformedSegments;
    }

    void setMaterial(PhongMaterial* material) {
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

    PhongMaterial* getMaterial() const {
        return material;
    }

protected:
    virtual std::vector<LineSegment> allIntersectPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;

private:
    PhongMaterial* material;
};
