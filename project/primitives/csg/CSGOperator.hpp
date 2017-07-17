#pragma once

#include "../../nodes/GeometryNode.hpp"
#include "../Mesh.hpp"

class CSGOperator : public Primitive {
public:
    CSGOperator(GeometryNode* leftNode, GeometryNode* rightNode)
        : left(leftNode->m_primitive), right(rightNode->m_primitive) {

        assert(dynamic_cast<Mesh*>(left) == nullptr && "CSG doesn't support meshes");
        assert(dynamic_cast<Mesh*>(right) == nullptr && "CSG doesn't support meshes");

        left->setTransform(leftNode->getTransform(), leftNode->getInvTransform());
        right->setTransform(rightNode->getTransform(), rightNode->getInvTransform());
    }

    virtual Intersection getClosestIntersection(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    virtual std::vector<LineSegment> getCSGSegments(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const override;

    virtual AABB getAABB() const override;

protected:
    virtual bool isInsideTransformed(const glm::dvec3& point) const override = 0;

    virtual std::vector<LineSegment> getCSGSegmentsPostTransform(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    ) const = 0;

    Primitive* left;
    Primitive* right;
};



