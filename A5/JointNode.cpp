// Winter 2019

#include "JointNode.hpp"

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string &name)
    : SceneNode(name)
{
  m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode()
{
}
//---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
  this->curXRot = init;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
  this->curYRot = init;
}

void JointNode::rotateX(double deltaX)
{
  deltaX = glm::degrees(deltaX);

  double oldX = this->curXRot;
  double newX = glm::clamp(oldX + deltaX, this->m_joint_x.min, this->m_joint_x.max);
  this->curXRot = newX;
}

void JointNode::rotateY(double deltaY)
{
  deltaY = glm::degrees(deltaY);

  double oldY = this->curYRot;
  double newY = glm::clamp(oldY + deltaY, this->m_joint_y.min, this->m_joint_y.max);
  this->curYRot = newY;
}

double JointNode::getCurXRot() const
{
  return this->curXRot;
}

double JointNode::getCurYRot() const
{
  return this->curYRot;
}

void JointNode::reset()
{
  this->curXRot = this->m_joint_x.init;
  this->curYRot = this->m_joint_y.init;
}

void JointNode::setRotXY(double xRot, double yRot)
{
  this->curXRot = xRot;
  this->curYRot = yRot;
}