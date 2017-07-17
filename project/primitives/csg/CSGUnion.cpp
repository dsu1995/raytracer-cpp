#include <glm/ext.hpp>
#include "CSGUnion.hpp"

CSGUnion::CSGUnion(GeometryNode* left, GeometryNode* right)
    : CSGOperator(left, right) {}

std::vector<LineSegment> CSGUnion::getCSGSegmentsPostTransform(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
) const {
    std::vector<LineSegment> leftSegments = left->getCSGSegments(rayOrigin, rayDirection);
    std::vector<LineSegment> rightSegments = right->getCSGSegments(rayOrigin, rayDirection);

    std::vector<LineSegment> output;

    size_t i = 0;
    size_t j = 0;
    while (i < leftSegments.size() && j < rightSegments.size()) {
        LineSegment& a = leftSegments.at(i);
        LineSegment& b = rightSegments.at(j);

        double a_near_dist = glm::distance2(rayOrigin, a.near.point);
        double a_far_dist = glm::distance2(rayOrigin, a.far.point);

        double b_near_dist = glm::distance2(rayOrigin, b.near.point);
        double b_far_dist = glm::distance2(rayOrigin, b.far.point);

        /*
         * a      ----
         * b ----
         */
        if (b_far_dist <= a_near_dist) {
            output.push_back(b);
            j++;
        }
        /*
         * a    ----
         * b  ----
         */
        else if (b_near_dist <= a_near_dist &&
                 a_near_dist <= b_far_dist &&
                 b_far_dist <= a_far_dist) {

            a.near = b.near;
            j++;
        }
        /*
         * a    ----
         * b  --------
         */
        else if (b_near_dist <= a_near_dist && a_far_dist <= b_far_dist) {
            i++;
        }
        /*
         * a    ----
         * b     --
         */
        else if (a_near_dist <= b_near_dist && b_far_dist <= a_far_dist) {
            j++;
        }
        /*
         * a    ----
         * b      ----
         */
        else if (a_near_dist <= b_near_dist &&
                 b_near_dist <= a_far_dist &&
                 a_far_dist <= b_far_dist) {

            b.near = a.near;
            i++;
        }
        /*
         * a  ----
         * b        ----
         */
        else if (a_far_dist <= b_near_dist) {
            output.push_back(a);
            i++;
        }
        else {
            assert(false);
        }
    }

    for ( ; i < leftSegments.size(); i++) {
        const LineSegment& a = leftSegments.at(i);
        output.push_back(a);
    }

    for ( ; j < rightSegments.size(); j++) {
        const LineSegment& b = rightSegments.at(j);
        output.push_back(b);
    }

    return output;
}

bool CSGUnion::isInsideTransformed(const glm::dvec3& point) const {
    return left->isInside(point) || right->isInside(point);
}

CSGUnion* CSGUnion::clone() const {
    return new CSGUnion(*this);
}
