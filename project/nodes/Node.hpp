#pragma once

#include <string>
#include <vector>

#include "../Transformable.hpp"
#include "../geometry/Geometry.hpp"


class Node : public Transformable {
public:
    Node(
        const std::string& name,
        Geometry* geometry = nullptr
    );

    virtual ~Node() {}

    const std::string& getName() const;

    const std::vector<Node*>& getChildren() const;

    void addChild(Node* child);

    Geometry* getGeometry() const;

private:
    std::string name;

    Geometry* geometry;

    std::vector<Node*> children;
};



