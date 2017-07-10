#pragma once

#include "../nodes/GeometryNode.hpp"
#include "Mesh.hpp"

class CSGOperator : public Primitive {
public:
    CSGOperator(GeometryNode* leftNode, GeometryNode* rightNode)
        : left(leftNode->m_primitive), right(rightNode->m_primitive) {

        assert(dynamic_cast<Mesh*>(left) == nullptr && "CSG doesn't support meshes");
        assert(dynamic_cast<Mesh*>(right) == nullptr && "CSG doesn't support meshes");

        left->setTransform(leftNode->getTransform(), leftNode->getInvTransform());
        right->setTransform(rightNode->getTransform(), rightNode->getInvTransform());
    }


protected:
    Primitive* left;
    Primitive* right;
};



