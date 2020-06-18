#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include "SceneNode.hpp"
#include <string>
#include <glm/gtx/io.hpp>

enum FrameType
{
  TRANSLATE,
  ROTATE_X,
  ROTATE_Y,
  ROTATE_Z,
  SCALE,
  ENABLE_DISABLE,
  SOUND,
  STOP_SOUND
};

class SceneNode;

class Frame
{
  public:
  Frame(){}
  Frame(Frame * b) {
    this->fType = b->fType;
    this->transformDataFrom = b->transformDataFrom;
    this->transformDataTo = b->transformDataTo;
    this->magDataFrom = b->magDataFrom;
    this->transformDataTo = b->transformDataTo;
    this->magDataFrom = b->magDataFrom;
    this->magDataTo = b->magDataTo;
    this->startTime = b->startTime;
    this->endTime = b->endTime;
    this->strData = b->strData;
    this->boolData = b->boolData;
    this->hasTriggered = b->hasTriggered;
    this->node = b->node;
  }

  FrameType fType = FrameType::TRANSLATE;
  glm::vec3 transformDataFrom;
  glm::vec3 transformDataTo;
  float magDataFrom;
  float magDataTo;

  float startTime;
  float endTime;
  
  std::string strData;
  bool boolData;

  SceneNode * node = nullptr;
  bool hasTriggered = false;

  void SetStartEndTime(float startTime, float endTime);

  // for translation, scale
  void SetTransformationData(glm::vec3 start, glm::vec3 end);

  // For rotation
  void SetMagData(float start, float end);

  void SetFrameType(FrameType fType);

  void SetSceneNode(SceneNode * node);

  void SetStrData(std::string str);
  void SetBoolData(bool set);

  void PrintFrameData();
};
