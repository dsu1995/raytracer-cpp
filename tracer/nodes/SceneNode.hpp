#pragma once

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>
#include "../Intersection.hpp"
#include "../Transformable.hpp"

enum class NodeType {
    SceneNode,
    GeometryNode,
    JointNode
};

class SceneNode : public Transformable {
public:
    SceneNode(const std::string& name);

    SceneNode(const SceneNode& other);

    virtual ~SceneNode();

    int totalSceneNodes() const;

    void add_child(SceneNode* child);

    void remove_child(SceneNode* child);


    virtual Intersection intersect(
        const glm::dvec3& rayOrigin,
        const glm::dvec3& rayDirection
    );

    friend std::ostream& operator<<(std::ostream& os, const SceneNode& node);

    std::list<SceneNode*> children;

    NodeType m_nodeType;
    std::string m_name;
    unsigned int m_nodeId;


private:
    // The number of SceneNode instances.
    static unsigned int nodeInstanceCount;
};
