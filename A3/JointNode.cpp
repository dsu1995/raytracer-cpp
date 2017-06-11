#include "JointNode.hpp"

#include <algorithm>

using std::max;
using std::min;

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;

	m_joint_x.cur = init;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;

	m_joint_y.cur = init;
}

void JointNode::reset() {
	m_joint_x.cur = m_joint_x.init;
	m_joint_y.cur = m_joint_y.init;
}

void JointNode::rotateX(double delta) {
	m_joint_x.cur =
		max(m_joint_x.min, min(m_joint_x.cur + delta, m_joint_x.max));
}

void JointNode::rotateY(double delta) {
	m_joint_y.cur =
		max(m_joint_y.min, min(m_joint_y.cur + delta, m_joint_y.max));
}
