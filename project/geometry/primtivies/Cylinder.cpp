#define GLM_SWIZZLE
#include "Cylinder.hpp"

#include <algorithm>
#include <glm/ext.hpp>

#include "../../polyroots.hpp"

using glm::dvec3;

/**
 * https://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html#SECTION00023200000000000000
 *
 * Models a cylinder centered at (0,0,0), with radius = 1, z_max = 1, z_min = -1
 */
Intersection Cylinder::intersect(
    const dvec3& rayOrigin,
    const dvec3& rayDirection
) const {
    const double z_min = -1;
    const double z_max = 1;

    double xd = rayDirection.x;
    double yd = rayDirection.y;
    double zd = rayDirection.z;
    double xe = rayOrigin.x;
    double ye = rayOrigin.y;
    double ze = rayOrigin.z;

    double A = xd * xd + yd * yd;
    double B = 2 * (xe * xd + ye * yd);
    double C = xe * xe + ye * ye - 1;

    Intersection closest;

    { // intersection with sides of cylinder
        double roots[2];
        size_t numRoots = quadraticRoots(A, B, C, roots);
        for (size_t i = 0; i < numRoots; i++) {
            double t = roots[i];
            if (t >= 0) {
                dvec3 intersectionPoint = rayOrigin + t * rayDirection;
                if (z_min < intersectionPoint.z && intersectionPoint.z < z_max) {
                    dvec3 normal(intersectionPoint.xy(), 0);
                    Intersection intersection(intersectionPoint, normal);
                    closest = Intersection::min(rayOrigin, closest, intersection);
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
                Intersection intersection(intersectionPoint, normal);
                closest = Intersection::min(rayOrigin, closest, intersection);
            }
        }
    }

    { // intersection with bottom
        double t = (z_min - ze) / zd;
        if (t >= 0) {
            dvec3 intersectionPoint = rayOrigin + t * rayDirection;
            if (glm::length2(intersectionPoint.xy()) <= 1) {
                dvec3 normal(0, 0, -1);
                Intersection intersection(intersectionPoint, normal);
                closest = Intersection::min(rayOrigin, closest, intersection);
            }
        }
    }

    return closest;
}
