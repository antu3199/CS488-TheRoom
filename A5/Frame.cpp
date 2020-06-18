#include "Frame.hpp"

void Frame::SetStartEndTime(float startTime, float endTime) {
    this->startTime = startTime;
    this->endTime = endTime;
}

// for translation, scale
void Frame::SetTransformationData(glm::vec3 start, glm::vec3 end) {
  this->transformDataFrom = start;
  this->transformDataTo = end;
}

// For rotation
void Frame::SetMagData(float start, float end) {
  this->magDataFrom = start;
  this->magDataTo = end;
}

void Frame::SetFrameType(FrameType fType) {
  this->fType = fType;
}

void Frame::SetSceneNode(SceneNode * node) {
  this->node = node;
}

void Frame::PrintFrameData() {
  if (this->node != nullptr) {
    std::cout << "Node Name: " << node->m_name << std::endl;
  } else {
    std::cout << "Node is not set!" << std::endl;
  }
  
  std::cout << "fType: " << fType << std::endl;
  std::cout << "Start time: " << startTime << " " << endTime << std::endl;
  std::cout << "TransformFrom: " << this->transformDataFrom << " " << this->transformDataTo << std::endl;
  std::cout << "magFrom: " << this->magDataFrom  << " " << this->magDataTo << std::endl;
}

void Frame::SetStrData(std::string str) {
  this->strData = str;
}

void Frame::SetBoolData(bool set){
  this->boolData = set;
}
