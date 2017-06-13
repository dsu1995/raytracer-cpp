#include <cmath>
#include "trackball.hpp"
#include <glm/gtx/norm.hpp>


using glm::vec2;
using glm::vec3;
using glm::mat4;

namespace trackball {
    /**
     * void vCalcRotVec(float fNewX, float fNewY,
     *                  float fOldX, float fOldY,
     *                  float fDiameter);
     *    Calculates a rotation vector based on mouse motion over
     *    a virtual trackball.
     *    The fNew and fOld mouse positions
     *    should be in 'trackball' space. That is, they have been
     *    transformed into a coordinate system centered at the middle
     *    of the trackball. fNew, fOld, and fDiameter must all be specified
     *    in the same units (pixels for example).
     * Parameters: fNewX, fNewY - New mouse position in trackball space.
     *                            This is the second point along direction
     *                            of rotation.
     *             fOldX, fOldY - Old mouse position in trackball space.
     *                            This is the first point along direction
     *                            of rotation.
     */
    vec3 calculateRotationVector(
        const vec2& newVec,
        const vec2& oldVec,
        float diameter
    ) {
        vec2 temp = newVec * 2.0f / diameter;
        vec3 newVec3(temp, 1 - glm::length2(temp));

        // If the Z component is less than 0, the mouse point
        // falls outside of the trackball which is interpreted
        // as rotation about the Z axis.
        if (newVec3.z < 0.0) {
            newVec3 /= sqrt(1.0 - newVec3.z);
            newVec3.z = 0;
        }
        else {
            newVec3.z = sqrtf(newVec3.z);
        }

        // Vector pointing from center of virtual trackball to old mouse position
        vec2 temp2 = oldVec * 2.0f / diameter;
        vec3 oldVec3(temp2, 1 - glm::length2(temp2));
        if (oldVec3.z < 0.0) {
            oldVec3 /= sqrt(1.0 - oldVec3.z);
            oldVec3.z = 0;
        }
        else {
            oldVec3.z = sqrtf(oldVec3.z);
        }

        return glm::cross(oldVec3, newVec3);
    }

    mat4 rotationAroundAxisMatrix(float radians, const vec3& axis) {
        glm::mat4 R;

        if (-0.000001 < radians && radians < 0.000001) {
            return R;
        }

        vec3 normalizedAxis = glm::normalize(axis);

        float x = normalizedAxis.x;
        float y = normalizedAxis.y;
        float z = normalizedAxis.z;

        float c = cosf(radians);
        float s = sinf(radians);

        R[0][0] = c + x*x*(1-c);
        R[1][0] = x*y*(1-c) - z*s;
        R[2][0] = x*z*(1-c) + y*s;
        R[0][1] = y*x*(1-c) + z*s;
        R[1][1] = c + y*y*(1-c);
        R[2][1] = y*z*(1-c) - x*s;
        R[0][2] = z*x*(1-c) - y*s;
        R[1][2] = z*y*(1-c) + x*s;
        R[2][2] = c + z*z*(1-c);
        return R;
    }
}
