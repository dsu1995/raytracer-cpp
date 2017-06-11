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

	rotate('x', float(init));
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;

	m_joint_y.cur = init;

	rotate('y', float(init));
}

void JointNode::reset() {
	m_joint_x.cur = m_joint_x.init;
	m_joint_y.cur = m_joint_y.init;
}

void JointNode::rotateX(double delta) {
	double cur = m_joint_x.cur;
	double next =
		max(m_joint_x.min, min(m_joint_x.cur + delta, m_joint_x.max));
	m_joint_x.cur = next;

	rotate('x', float(next - cur));
}

void JointNode::rotateY(double delta) {
	double cur = m_joint_y.cur;
	double next =
		max(m_joint_y.min, min(m_joint_y.cur + delta, m_joint_y.max));
	m_joint_y.cur = next;

	rotate('y', float(next - cur));
}
