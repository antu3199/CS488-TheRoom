// Winter 2019

#pragma once

#include "SceneNode.hpp"
#include <glm/glm.hpp>

class JointNode : public SceneNode
{
public:
  JointNode(const std::string &name);
  virtual ~JointNode();

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange
  {
    double min, init, max;
  };

  JointRange m_joint_x, m_joint_y;

  void rotateX(double deltaX);
  void rotateY(double deltaY);
  void reset();
  double getCurXRot() const;
  double getCurYRot() const;

  void setRotXY(double xRot, double yRot);

private:
  double curXRot;
  double curYRot;
};
