#include <glm/ext.hpp>
#include "CSGUnion.hpp"
#include "Mesh.hpp"

CSGUnion::CSGUnion(GeometryNode* left, GeometryNode* right)
    : CSGOperator(left, right) {}

std::vector<LineSegment> CSGUnion::allIntersectPostTransform(
    const glm::dvec3& rayOrigin,
    const glm::dvec3& rayDirection
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

        if (output.empty()) {
            if (a_near_dist < b_near_dist) {
                output.push_back(a);
                i++;
            }
            else {
                output.push_back(b);
                j++;
            }
        }
        else {
            LineSegment& top = output.back();
            double top_far_dist = glm::distance2(rayOrigin, top.far.point);

            // a smaller
            if (a_near_dist < b_near_dist) {
                // disjoint
                if (top_far_dist < a_near_dist) {
                    output.push_back(a);
                }
                else {
                    if (top_far_dist < a_far_dist) {
                        top.far = a.far;
                    }
                }
                i++;
            }
            // b smaller
            else {
                // disjoint
                if (top_far_dist < b_near_dist) {
                    output.push_back(b);
                }
                else {
                    if (top_far_dist < b_far_dist) {
                        top.far = b.far;
                    }
                }
                j++;
            }
        }
    }

    for ( ;i < leftSegments.size(); i++) {
        const LineSegment& a = leftSegments.at(i);
        double a_near_dist = glm::distance2(rayOrigin, a.near.point);
        double a_far_dist = glm::distance2(rayOrigin, a.far.point);

        if (output.empty()) {
            output.push_back(a);
        }
        else {
            LineSegment& top = output.back();
            double top_far_dist = glm::distance2(rayOrigin, top.far.point);

            // disjoint
            if (top_far_dist < a_near_dist) {
                output.push_back(a);
            }
            else {
                if (top_far_dist < a_far_dist) {
                    top.far = a.far;
                }
            }
        }
    }

    for ( ; j < rightSegments.size(); j++) {
        const LineSegment& b = rightSegments.at(j);
        double b_near_dist = glm::distance2(rayOrigin, b.near.point);
        double b_far_dist = glm::distance2(rayOrigin, b.far.point);

        if (output.empty()) {
            output.push_back(b);
        }
        else {
            LineSegment& top = output.back();
            double top_far_dist = glm::distance2(rayOrigin, top.far.point);
            // disjoint
            if (top_far_dist < b_near_dist) {
                output.push_back(b);
            }
            else {
                if (top_far_dist < b_far_dist) {
                    top.far = b.far;
                }
            }
        }
    }

    return output;
}
