// Winter 2019

#include "SceneNode.hpp"

#include "cs488-framework/MathUtils.hpp"

#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/io.hpp>

using namespace glm;

// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;

//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string &name)
    : m_name(name),
      m_nodeType(NodeType::SceneNode),
      trans(mat4()),
      isSelected(false),
      m_nodeId(nodeInstanceCount++)
{
}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode &other)
    : m_nodeType(other.m_nodeType),
      m_name(other.m_name),
      trans(other.trans),
      invtrans(other.invtrans)
{
  for (SceneNode *child : other.children)
  {
    this->children.push_front(new SceneNode(*child));
  }
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
  for (Frame *frame : frames)
  {
    delete frame;
  }
  frames.clear();

  for (SceneNode *child : children)
  {
    delete child;
  }

  for (AnimLoop * loop : this->animLoops) {
    delete loop;
  }
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4 &m)
{
  trans = m;
  invtrans = m;
}

//---------------------------------------------------------------------------------------
const glm::mat4 &SceneNode::get_transform() const
{
  return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4 &SceneNode::get_inverse() const
{
  return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode *child)
{
  children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode *child)
{
  children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle)
{
  vec3 rot_axis;

  switch (axis)
  {
  case 'x':
    rot_axis = vec3(1, 0, 0);
    this->animRot.x = angle;
    break;
  case 'y':
    rot_axis = vec3(0, 1, 0);
    this->animRot.y = angle;
    break;
  case 'z':
    rot_axis = vec3(0, 0, 1);
    this->animRot.z = angle;
    break;
  default:
    break;
  }
  mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
  trans = rot_matrix * trans;
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 &amount)
{
  trans = glm::scale(amount) * trans;
  this->animScale = amount;
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3 &amount)
{
  trans = glm::translate(amount) * trans;
  this->animPos = amount;
}

//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const
{
  return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const SceneNode &node)
{

  //os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
  switch (node.m_nodeType)
  {
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
  os << "]";

  return os;
}

glm::mat4 SceneNode::getAnimMatrix()
{
  glm::mat4 trans = glm::translate(this->animPos);
  glm::mat4 rotX = glm::rotate(degreesToRadians(this->animRot.x), glm::vec3(1, 0, 0));
  glm::mat4 rotY = glm::rotate(degreesToRadians(this->animRot.y), glm::vec3(0, 1, 0));
  glm::mat4 rotZ = glm::rotate(degreesToRadians(this->animRot.z), glm::vec3(0, 0, 1));
  glm::mat4 scale = glm::scale(this->animScale);

  return trans * rotX * rotY * rotZ * scale;
}

void SceneNode::getAnimMetaData()
{
  if (frames.size() == 0)
  {
    return;
  }

  std::sort(frames.begin(), frames.end(), [](Frame *a, Frame *b) {
    return a->startTime < b->startTime;
  });

  this->animOffset = frames[0]->startTime;
  float endTime = 0;
  for (int i = 0; i < frames.size(); i++)
  {
    endTime = std::max(endTime, frames[i]->endTime);
  }
  this->animDuration = endTime - frames[0]->startTime;
}

  void SceneNode::SetCurAnimLoop(float absTime){
    for (int i = 0; i < animLoops.size(); i++) {
      AnimLoop * loop = animLoops[i];
      if (absTime >= loop->startTime && absTime <= loop->endTime) {
        this->curAnimLoop = loop;
        return;
      }
    }
    this->curAnimLoop = nullptr;
  }

void AnimLoop::Init(std::vector<Frame *> & f) {
  maxCount = 0;
  float minTime = INT_MAX;
  float maxTime = 0.0f;
  for (int i = 0; i < f.size(); i++) {
    Frame * frame = f[i];
    if (this->inRange(frame->startTime, frame->endTime)) {
      this->maxCount++;
      minTime = std::min(minTime, frame->startTime);
      maxTime = std::max(maxTime, frame->endTime);
    }
  }

  this->animDuration = maxTime - minTime;
}