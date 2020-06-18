#include "ParticleSystem.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include <iostream>
#include <glm/gtx/io.hpp>
#include <glm/ext.hpp>
#include <algorithm>

#include "A5.hpp"

ParticleSystem::ParticleSystem()
{
}

void ParticleSystem::Init(ShaderProgram *shader, glm::mat4 *viewMatrix, glm::mat4 *projMatrix)
{
  this->shader = shader;
  this->viewMatrix = viewMatrix;
  this->projMatrix = projMatrix;

  vertexPositionData = new GLfloat[MaxParticles * 4];
  vertexColorData = new GLubyte[MaxParticles * 4];

  for (int i = 0; i < MaxParticles; i++)
  {
    particlesContainer[i].life = -1.0f;
    particlesContainer[i].cameraDistance = -1.0f;
  }

  glGenVertexArrays(1, &m_vao_meshData);
  glBindVertexArray(m_vao_meshData);

  glGenBuffers(1, &billboardVertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

  glGenBuffers(1, &positionBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void ParticleSystem::SetTexture(std::string path)
{
  Image image;
  texture = image.loadImagePNG(A5::getAssetPath(path.c_str()));
}

void ParticleSystem::Update(float deltaTime, glm::vec3 position, glm::vec3 cameraPosition, glm::mat4 model)
{
  if (this->isPlaying == false)
  {
    return;
  }

  float maxLife = 1.0f;

  if (this->isSpawning)
  {
    float rate = 100.0f;
    int particleSpawn = (int)(deltaTime * rate);
    if (particleSpawn > (int)(0.016f * rate))
      particleSpawn = (int)(0.016f * rate);

    for (int i = 0; i < particleSpawn; i++)
    {
      int particleIndex = FindUnusedParticle();
      particlesContainer[particleIndex].life = maxLife; 
      particlesContainer[particleIndex].pos = position;

      float spread = 0.3f;
      glm::vec3 randomdir = glm::vec3(
          (rand() % 2000 - 1000.0f) / 1000.0f,
          (rand() % 2000 - 1000.0f) / 1000.0f,
          (rand() % 2000 - 1000.0f) / 1000.0f);

      particlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;

      particlesContainer[particleIndex].color.r = 255;
      particlesContainer[particleIndex].color.g = 255;
      particlesContainer[particleIndex].color.b = 255;
      particlesContainer[particleIndex].color.a = 125;

      float ySpeed = ((std::abs(randomdir.x) + std::abs(randomdir.z))) * 0.3f;

      particlesContainer[particleIndex].pos = glm::vec3(position.x + (randomdir.x * spread), position.y, position.z);
      particlesContainer[particleIndex].speed = glm::vec3(position.x - particlesContainer[particleIndex].pos.x + (randomdir.x) * 0.01f, ySpeed, 0);
      
    }
  }

  // Simulate all particles
  particleCount = 0;
  for (int i = 0; i < MaxParticles; i++)
  {

    Particle &p = particlesContainer[i]; // shortcut

    if (p.life > 0.0f)
    {

      // Decrease life
      p.life -= deltaTime;
      if (p.life > 0.0f)
      {

        p.pos += p.speed * (float)deltaTime;
        p.color.a -= 50.0f * (float) deltaTime;

        p.cameraDistance = glm::distance(p.pos, cameraPosition);

        // Fill the GPU buffer
        vertexPositionData[4 * particleCount + 0] = p.pos.x;
        vertexPositionData[4 * particleCount + 1] = p.pos.y;
        vertexPositionData[4 * particleCount + 2] = p.pos.z;

        vertexPositionData[4 * particleCount + 3] = p.size;

        vertexColorData[4 * particleCount + 0] = p.color.r;
        vertexColorData[4 * particleCount + 1] = p.color.g;
        vertexColorData[4 * particleCount + 2] = p.color.b;
        vertexColorData[4 * particleCount + 3] = p.color.a;
      }
      else
      {
        // Particles that just died will be put at the end of the buffer in SortParticles();
        p.cameraDistance = -1.0f;
        p.pos = glm::vec3(9999999, 999999, 999999);
      }

      particleCount++;
    }
  }

  this->shader->enable();
  {
    glBindVertexArray(m_vao_meshData);
    glm::mat4 projMatrix = *this->projMatrix;
    glm::mat4 viewMatrix = *this->viewMatrix;
    glm::mat4 viewProjMatrix = projMatrix * viewMatrix;

    // Vertex shader
    GLuint cameraRightLoc = shader->getUniformLocation("cameraRight_WS");
    GLuint cameraUpLoc = shader->getUniformLocation("cameraUp_WS");
    GLuint viewProjLoc = shader->getUniformLocation("ViewProj");

    glm::vec3 cameraRight = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glm::vec3 cameraUp = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    // Same as the billboards tutorial
    glUniform3fv(cameraRightLoc, 1, value_ptr(cameraRight));
    glUniform3fv(cameraUpLoc, 1, value_ptr(cameraUp));

    glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);

    CHECK_GL_ERRORS;
  }
  this->shader->disable();
}

void ParticleSystem::Draw()
{

glEnable(GL_BLEND);
glDepthMask(GL_FALSE);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


  glBindVertexArray(m_vao_meshData);

  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Vertex Location
  GLuint vertexAttribLocation = shader->getAttribLocation("quadVertices");
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, vertexPositionData);
  glEnableVertexAttribArray(vertexAttribLocation);

  glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
  glVertexAttribPointer(vertexAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  GLuint xyzsAttribLocation = shader->getAttribLocation("xyzs");
  glEnableVertexAttribArray(xyzsAttribLocation);
  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glVertexAttribPointer(xyzsAttribLocation, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);

  GLuint colorAttribLocation = shader->getAttribLocation("color");
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLubyte) * 4, vertexColorData);

  glEnableVertexAttribArray(colorAttribLocation);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(colorAttribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void *)0);

  glVertexAttribDivisor(vertexAttribLocation, 0);
  glVertexAttribDivisor(xyzsAttribLocation, 1);
  glVertexAttribDivisor(colorAttribLocation, 1);

  // Use our shader
  this->shader->enable();
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
  this->shader->disable();

  glDisableVertexAttribArray(vertexAttribLocation);
  glDisableVertexAttribArray(xyzsAttribLocation);
  glDisableVertexAttribArray(colorAttribLocation);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

glDepthMask(GL_TRUE);
glDisable(GL_BLEND);
}

int ParticleSystem::FindUnusedParticle()
{

  for (int i = lastUsedParticleIndex; i < MaxParticles; i++)
  {
    if (particlesContainer[i].life < 0)
    {
      lastUsedParticleIndex = i;
      return i;
    }
  }

  for (int i = 0; i < lastUsedParticleIndex; i++)
  {
    if (particlesContainer[i].life < 0)
    {
      lastUsedParticleIndex = i;
      return i;
    }
  }

  return 0; // All particles are taken, override the first one
}

void ParticleSystem::SortParticles()
{
  std::sort(&particlesContainer[0], &particlesContainer[MaxParticles], [](Particle &a, Particle &b) {
    if (a.cameraDistance == b.cameraDistance) {
      return a.life < b.life;
    }
    return a.cameraDistance > b.cameraDistance;
  });
}
