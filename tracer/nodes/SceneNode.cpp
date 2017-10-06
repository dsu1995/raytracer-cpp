#include "SceneNode.hpp"

#include <iostream>
#include <sstream>

using namespace std;

#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;


// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
    : m_name(name),
      m_nodeType(NodeType::SceneNode),
      m_nodeId(nodeInstanceCount++) {}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode& other)
    : m_nodeType(other.m_nodeType),
      m_name(other.m_name)
{
    for (SceneNode* child : other.children) {
        this->children.push_front(new SceneNode(*child));
    }
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
    for (SceneNode* child : children) {
        delete child;
    }
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
    children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child) {
    children.remove(child);
}

//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
    return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const SceneNode& node) {

    //os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
    switch (node.m_nodeType) {
        case NodeType::SceneNode:
            os << "SceneNode";
            break;
        case NodeType::GeometryNode:
            os << "GeometryNode";
            break;
        case NodeType::JointNode:
            os << "JointNode";
            break;
    }
    os << ":[";

    os << "name:" << node.m_name << ", ";
    os << "id:" << node.m_nodeId;

    os << "]\n";
    return os;
}

Intersection SceneNode::intersect(const glm::dvec3& rayOrigin, const glm::dvec3& rayDirection) {
    Ray transformedRay = transformRay(rayOrigin, rayDirection);
    const dvec3& newOrigin = transformedRay.rayOrigin;
    const dvec3& newDirection = transformedRay.rayDirection;


    Intersection closest;
    for (SceneNode* child: children) {
        Intersection intersection = child->intersect(newOrigin, newDirection);
        closest = Intersection::min(newOrigin, closest, intersection);
    }
    return transformIntersectionBack(closest);
}
