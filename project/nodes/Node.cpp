#include "Node.hpp"

using std::string;

Node::Node(
    const string& name,
    Geometry* geometry
) : name(name),
    geometry(geometry),
    children() {}

const std::string& Node::getName() const {
    return name;
}

const std::vector<Node*>& Node::getChildren() const {
    return children;
}

void Node::addChild(Node* child) {
    children.push_back(child);
}

Geometry* Node::getGeometry() const {
    return geometry;
}




