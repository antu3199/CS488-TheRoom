// Winter 2019

#pragma once

#include "SceneNode.hpp"

struct LightSource
{
  glm::vec3 color;
  float intensity;
};

class LightNode : public SceneNode {
public:
	LightNode(
		const std::string & name
	);
  LightSource lightInfo;
};
