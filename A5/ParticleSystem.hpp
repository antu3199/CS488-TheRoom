#pragma once

#include <glm/glm.hpp>
#include "cs488-framework/OpenGLImport.hpp"
#include <string>
#include "cs488-framework/ShaderProgram.hpp"
#include <algorithm>
#include "Image.hpp"
#include <GLFW/glfw3.h>

const int MaxParticles = 2000;

// TODO: CHANGE THIS.
struct Particle
{
  glm::vec3 pos;
  glm::vec3 speed;
  glm::vec4 color;
  float size;
  float life;
  float cameraDistance;
};

class ParticleSystem
{
public:
  ParticleSystem();
  void Init(ShaderProgram *shader, glm::mat4 *viewMatrix, glm::mat4 *projMatrix);

  void Update(float deltaTime, glm::vec3 position, glm::vec3 cameraPos, glm::mat4 model);
  void Draw();

  Particle particlesContainer[MaxParticles];
  int lastUsedParticleIndex = 0;

  std::string name;
  void SetTexture(std::string path);

  bool isPlaying = true;
  bool isSpawning = true;

private:
  ShaderProgram *shader;

  int FindUnusedParticle();
  void SortParticles();

  GLfloat *vertexPositionData;
  GLubyte *vertexColorData;
  GLfloat quadVertices[12] = {
      -0.5f,
      -0.5f,
      0.0f,
      0.5f,
      -0.5f,
      0.0f,
      -0.5f,
      0.5f,
      0.0f,
      0.5f,
      0.5f,
      0.0f,
  };

  GLuint billboardVertexBuffer;
  GLuint positionBuffer;
  GLuint colorBuffer;
  int particleCount = 0;
  GLuint texture;

  glm::mat4 *viewMatrix = nullptr;
  glm::mat4 *projMatrix = nullptr;

  GLuint m_vao_meshData;
};