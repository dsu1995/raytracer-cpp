#define GLM_SWIZZLE
#include "Cone.hpp"

#include <glm/ext.hpp>
#include <vector>
#include <algorithm>
#include "../../polyroots.hpp"

using glm::dvec3;

const double z_min = 0;
const double z_max = 1;

const glm::dvec3 objCenter(0, 0, 0.5);

/**
 * Ray-cone intersection:
 * https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html#SECTION00023200000000000000
 *
 * Surface Normal:
 * https://math.stackexchange.com/questions/807056/constructing-a-cone-and-its-normal-vectors-in-spherical-coordinates
 *
 * Models an inverted cone with its tip at (0,0,0), with base radius = 1, and height 1
 */
std::vector<Intersection> Cone::getIntersectionsPostTransform(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    double xd = rayDirection.x;
    double yd = rayDirection.y;
    double zd = rayDirection.z;
    double xe = rayOrigin.x;
    double ye = rayOrigin.y;
    double ze = rayOrigin.z;

    double A = xd * xd + yd * yd - zd * zd;
    double B = 2 * (xe * xd + ye * yd - ze * zd);
    double C = xe * xe + ye * ye - ze * ze;

    std::vector<Intersection> intersections;

    { // intersection with sides of cone
        double roots[2];
        size_t numRoots = quadraticRoots(A, B, C, roots);
        for (size_t i = 0; i < numRoots; i++) {
            double t = roots[i];
            if (t >= 0) {
                dvec3 intersectionPoint = rayOrigin + t * rayDirection;
                if (z_min <= intersectionPoint.z && intersectionPoint.z < z_max) {
                    dvec3 normal = intersectionPoint * dvec3(2, 2, -2);
                    Intersection intersection(intersectionPoint, normal, objCenter, *material);
                    intersections.push_back(intersection);
                }
            }
        }
    }

    { // intersection with top
        double t = (z_max - ze) / zd;
        if (t >= 0) {
            dvec3 intersectionPoint = rayOrigin + t * rayDirection;
            if (glm::length2(intersectionPoint.xy()) <= 1) {
                dvec3 normal(0, 0, 1);
                Intersection intersection(intersectionPoint, normal, objCenter, *material);
                intersections.push_back(intersection);
            }
        }
    }

    return intersections;
}

bool Cone::isInsideTransformed(const glm::dvec3& point) const {
    return (
        z_min <= point.z && point.z <= z_max &&
        glm::length2(point.xy()) <= point.z * point.z
    );
}
