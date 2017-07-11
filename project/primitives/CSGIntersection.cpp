#include "CSGIntersection.hpp"

#include <glm/ext.hpp>


CSGIntersection::CSGIntersection(GeometryNode* left, GeometryNode* right)
    : CSGOperator(left, right) {}


std::vector<LineSegment> CSGIntersection::allIntersectPostTransform(
    const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection
) const {
    std::vector<LineSegment> leftSegments = left->allIntersect(rayOrigin, rayDirection);
    std::vector<LineSegment> rightSegments = right->allIntersect(rayOrigin, rayDirection);

    std::vector<LineSegment> output;

    size_t i = 0;
    size_t j = 0;
    while (i < leftSegments.size() && j < rightSegments.size()) {
        const LineSegment& a = leftSegments.at(i);
        const LineSegment& b = rightSegments.at(j);

        double a_near_dist = glm::distance2(rayOrigin, a.near.point);
        double a_far_dist = glm::distance2(rayOrigin, a.far.point);

        double b_near_dist = glm::distance2(rayOrigin, b.near.point);
        double b_far_dist = glm::distance2(rayOrigin, b.far.point);

        /*
         * a      ----
         * b ----
         */
        if (b_far_dist <= a_near_dist) {
            j++;
        }
        /*
         * a    ----
         * b  ----
         */
        else if (b_near_dist <= a_near_dist &&
                 a_near_dist <= b_far_dist &&
                 b_far_dist <= a_far_dist) {

            output.push_back(LineSegment(a.near, b.far));
            j++;
        }
        /*
         * a    ----
         * b  --------
         */
        else if (b_near_dist <= a_near_dist && a_far_dist <= b_far_dist) {
            output.push_back(a);
            i++;
        }
        /*
         * a    ----
         * b     --
         */
        else if (a_near_dist <= b_near_dist && b_far_dist <= a_far_dist) {
            output.push_back(b);
            j++;
        }
        /*
         * a    ----
         * b      ----
         */
        else if (a_near_dist <= b_near_dist &&
                 b_near_dist <= a_far_dist &&
                 a_far_dist <= b_far_dist) {

            output.push_back(LineSegment(b.near, a.far));
            i++;
        }
        /*
         * a  ----
         * b        ----
         */
        else if (a_far_dist <= b_near_dist) {
            i++;
        }
        else {
            assert(false);
        }
    }

    return output;
}
