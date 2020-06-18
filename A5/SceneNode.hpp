// Winter 2019

#pragma once

#include "Material.hpp"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>
#include "Frame.hpp"
#include <vector>
#include <algorithm>
#include "ParticleSystem.hpp"

enum class NodeType
{
  SceneNode,
  GeometryNode,
  JointNode,
  LightNode
};

class Frame;

struct AnimLoop {
  float startTime = 0.0f;
  float endTime = 0.0f;
  int maxCount = 0;
  int curCount = 0;
  float animDuration = 0.0f;
  std::vector<Frame *> disabledFrames;

  AnimLoop(float startTime, float endTime) {
    this->startTime = startTime;
    this->endTime = endTime;
  }

  bool inRange(float time) {
    return time >= startTime && time <= endTime;
  }

  void Init(std::vector<Frame *> & f);

  bool inRange(float start, float end) {
    return start >= this->startTime && end <= this->endTime;
  }
};

class SceneNode
{
public:
  SceneNode(const std::string &name);

  SceneNode(const SceneNode &other);

  virtual ~SceneNode();

  int totalSceneNodes() const;

  const glm::mat4 &get_transform() const;
  const glm::mat4 &get_inverse() const;

  void set_transform(const glm::mat4 &m);

  void add_child(SceneNode *child);

  void remove_child(SceneNode *child);

  //-- Transformations:
  void rotate(char axis, float angle);
  void scale(const glm::vec3 &amount);
  void translate(const glm::vec3 &amount);

  friend std::ostream &operator<<(std::ostream &os, const SceneNode &node);

  bool isSelected;

  // Transformations
  glm::mat4 trans;
  glm::mat4 invtrans;

  std::list<SceneNode *> children;

  glm::vec3 animPos = glm::vec3();
  glm::vec3 animRot = glm::vec3();
  glm::vec3 animScale = glm::vec3(1.0f);

  glm::mat4 getAnimMatrix();
  bool isAnimating = false;
  float animOffset = 0;
  float animDuration = 0;

  void getAnimMetaData();

  NodeType m_nodeType;
  std::string m_name;
  unsigned int m_nodeId;
  std::vector<Frame *> frames;
  AnimLoop * curAnimLoop;
  std::vector<AnimLoop *> animLoops;
  
  ParticleSystem * particleSystem = nullptr;

  void SetCurAnimLoop(float absTime);

  bool enabled = true;
private:
  // The number of SceneNode instances.
  static unsigned int nodeInstanceCount;
};
