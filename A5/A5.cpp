// Winter 2019

#include "A5.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <glm/ext.hpp>
#include "ParticleSystem.hpp"

#include "Noise.hpp"

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;
static const double SCROLL_SPEED = 0.005;

//----------------------------------------------------------------------------------------
// Constructor
A5::A5(const std::string &luaSceneFile)
    : m_luaSceneFile(luaSceneFile),
      m_positionAttribLocation(0),
      m_normalAttribLocation(0),
      m_uvAttribLocation(0),
      m_vao_meshData(0),
      m_vbo_vertexPositions(0),
      m_vbo_vertexNormals(0)
{
}

//----------------------------------------------------------------------------------------
// Destsructor
A5::~A5()
{
  for (auto it = this->shaders.begin(); it != this->shaders.end(); it++)
  {
    delete it->second;
  }
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A5::init()
{
  this->rightClickActive = false;
  this->leftClickActive = false;
  this->middleClickActive = false;

  // After bear scene
 // this->absTime = 15;

  // TEST VARIABLES CHANGE ME
 // this->toggleShadows = false;
  //this->toggleNormalMap = false;

  // Set the background colour.
  glClearColor(0.85, 0.85, 0.85, 1.0);

  createShaderProgram();

  glGenVertexArrays(1, &m_vao_meshData);
  enableVertexShaderInputSlots();

  processLuaSceneFile(m_luaSceneFile);

  // Load and decode all .obj files at once here.  You may add additional .obj files to
  // this list in order to support rendering additional mesh types.  All vertex
  // positions, and normals will be extracted and stored within the MeshConsolidator
  // class.
  unique_ptr<MeshConsolidator> meshConsolidator(new MeshConsolidator{
      getAssetFilePath("uvcube.obj"),
      getAssetFilePath("plane.obj"),
      getAssetFilePath("uvsphere.obj"),
      getAssetFilePath("room/Floor.obj"),
      getAssetFilePath("room/Cube.001.obj"),
      getAssetFilePath("room/Template.006.obj"),
      getAssetFilePath("room/Template.005.obj"),
      getAssetFilePath("room/Template.003.obj"),
      getAssetFilePath("room/Circle.015.obj"),
      getAssetFilePath("room/Circle.010.obj"),
      getAssetFilePath("room/Cube.obj"),
      getAssetFilePath("room/Cylinder.obj"),
      getAssetFilePath("room/Plane.022.obj"),
      getAssetFilePath("room/Circle.014.obj"),
      getAssetFilePath("room/Plane.008.obj"),
      getAssetFilePath("room/Plane.023.obj"),
      getAssetFilePath("room/Plane.006.obj"),
      getAssetFilePath("room/BezierCircle.003.obj"),
      getAssetFilePath("room/BezierCircle.005.obj"),
      getAssetFilePath("room/Cube.003.obj"),
      getAssetFilePath("room/Cube.008.obj"),
      getAssetFilePath("room/Circle.009.obj"),
      getAssetFilePath("room/HalfSmallWall.obj"),
      getAssetFilePath("room/Plane.obj"),
      getAssetFilePath("room/Plane.001.obj"),
      getAssetFilePath("room/Plane.002.obj"),
      getAssetFilePath("room/Plane.007.obj"),
      getAssetFilePath("room/Plane.013.obj"),
      getAssetFilePath("room/Plane.016.obj"),
      getAssetFilePath("room/Plane.021.obj"),
      getAssetFilePath("room/Plane.024.obj"),
      getAssetFilePath("room/Plane.026.obj"),
      getAssetFilePath("room/Plane.028.obj"),
      getAssetFilePath("room/Plane.042.obj"),
      getAssetFilePath("room/SmallDoor.obj"),
      //tv
      getAssetFilePath("objModels/tv.obj"),
      // bear
      getAssetFilePath("bear/b1.obj"),
      getAssetFilePath("bear/b2.obj"),
      getAssetFilePath("bear/b3.obj"),
      getAssetFilePath("bear/b4.obj"),
  });


  // Acquire the BatchInfoMap from the MeshConsolidator.
  meshConsolidator->getBatchInfoMap(m_batchInfoMap);

  // Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
  uploadVertexDataToVbos(*meshConsolidator);

  mapVboDataToVertexShaderInputLocations();

  initPerspectiveMatrix();

  initViewMatrix();

  initLightSources();

  this->resetAll();

  this->InitPass(*m_rootNode);
  this->PostInitPass();

  this->soundPlayer.Init();
}

//----------------------------------------------------------------------------------------
void A5::processLuaSceneFile(const std::string &filename)
{
  m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
  if (!m_rootNode)
  {
    std::cerr << "Could Not Open " << filename << std::endl;
  }
  else
  {
    this->mapJoints(*m_rootNode);
  }
}

void A5::initShaderProgram(std::string name, std::string vert, std::string frag, std::string geo)
{
  this->shaders[name] = new ShaderProgram();
  ShaderProgram *shader = this->shaders[name];
  shader->generateProgramObject();
  shader->attachVertexShader(getAssetFilePath(vert.c_str()).c_str());
  shader->attachFragmentShader(getAssetFilePath(frag.c_str()).c_str());
  if (geo != "")
  {
    shader->attachGeometryShader(getAssetFilePath(geo.c_str()).c_str());
  }
  shader->link();
}

//----------------------------------------------------------------------------------------
void A5::createShaderProgram()
{

  this->initShaderProgram("uvShader", "uvVertexShader.vs", "uvFragmentShader.fs");
  this->initShaderProgram("particleSystem", "particleSystem.vs", "particleSystem.fs");
  this->initShaderProgram("shadowDepthShader", "shadowDepthShader.vs", "shadowDepthShader.fs", "shadowDepthShader.gs");

  this->initShadowDepthShader();

  this->initShaderProgram("noiseShader", "noiseShader.vs", "noiseShader.fs");
}

void A5::initShadowDepthShader()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glGenFramebuffers(1, &shadowDepthMapFBO);
  glGenTextures(1, &shadowDepthCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, shadowDepthCubemap);

  for (unsigned int i = 0; i < 6; i++)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepthCubemap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//----------------------------------------------------------------------------------------
void A5::enableVertexShaderInputSlots()
{
  //-- Enable input slots for m_vao_meshData:
  for (auto it = this->shaders.begin(); it != this->shaders.end(); it++)
  {
    std::string key = it->first;
    ShaderProgram *shader = it->second;

    if (key == "uvShader")
    {
      {
        glBindVertexArray(m_vao_meshData);

        // Enable the vertex shader attribute location for "position" when rendering.
        m_positionAttribLocation = shader->getAttribLocation("position");
        glEnableVertexAttribArray(m_positionAttribLocation);

        // Enable the vertex shader attribute location for "normal" when rendering.
        m_normalAttribLocation = shader->getAttribLocation("normal");
        glEnableVertexAttribArray(m_normalAttribLocation);

        m_uvAttribLocation = shader->getAttribLocation("uv");
        glEnableVertexAttribArray(m_uvAttribLocation);

        m_tangentAttribLocation = shader->getAttribLocation("tangent");
        glEnableVertexAttribArray(m_tangentAttribLocation);

        m_bitangentAttribLocation = shader->getAttribLocation("bitangent");
        glEnableVertexAttribArray(m_bitangentAttribLocation);

        CHECK_GL_ERRORS;
      }
    }
    else if (key == "shadowDepthShader")
    {
      glBindVertexArray(m_vao_meshData);

      // Enable the vertex shader attribute location for "position" when rendering.
      m_positionAttribLocation = shader->getAttribLocation("position");
      glEnableVertexAttribArray(m_positionAttribLocation);
    }
    else if (key == "noiseShader")
    {
      glBindVertexArray(m_vao_meshData);

      // Enable the vertex shader attribute location for "position" when rendering.
      m_positionAttribLocation = shader->getAttribLocation("position");
      glEnableVertexAttribArray(m_positionAttribLocation);
    }
    else
    {

      //std::cout << "Unknown shader" << std::endl;
    }

    // Restore defaults
    glBindVertexArray(0);
  }
}

//----------------------------------------------------------------------------------------
void A5::uploadVertexDataToVbos(
    const MeshConsolidator &meshConsolidator)
{
  // Generate VBO to store all vertex position data
  {
    glGenBuffers(1, &m_vbo_vertexPositions);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
                 meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERRORS;
  }

  // Generate VBO to store all vertex normal data
  {
    glGenBuffers(1, &m_vbo_vertexNormals);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
                 meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERRORS;
  }

  {
    glGenBuffers(1, &m_vbo_vertexUv);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexUv);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexUVBytes(),
                 meshConsolidator.getVertexUVDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }

  {

    glGenBuffers(1, &m_vbo_vertexTangent);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexTangent);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexTangentBytes(),
                 meshConsolidator.getVertexTangentDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }

  {

    glGenBuffers(1, &m_vbo_vertexBitangent);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexBitangent);

    glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexBitangentBytes(),
                 meshConsolidator.getVertexBitangentDataPtr(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_GL_ERRORS;
  }
}

//----------------------------------------------------------------------------------------
void A5::mapVboDataToVertexShaderInputLocations()
{
  // Bind VAO in order to record the data mapping.
  glBindVertexArray(m_vao_meshData);

  // Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
  // "position" vertex attribute location for any bound vertex shader program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
  glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
  // "normal" vertex attribute location for any bound vertex shader program.
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
  glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexUv);
  glVertexAttribPointer(m_uvAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexTangent);
  glVertexAttribPointer(m_tangentAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexBitangent);
  glVertexAttribPointer(m_bitangentAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  //-- Unbind target, and restore default values:
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A5::initPerspectiveMatrix()
{
  float aspect = ((float)m_windowWidth) / m_windowHeight;
  float fov = degreesToRadians(60.0f);
  near = 0.1f;
  far = 100.0f;
  m_perpsective = glm::perspective(fov, aspect, near, far);

  float shadowAspect = (float)SHADOWMAP_WIDTH / (float)SHADOWMAP_HEIGHT;
  this->shadowProj = glm::perspective(glm::radians(90.0f), shadowAspect, near, far);
}

//----------------------------------------------------------------------------------------
void A5::initViewMatrix()
{
  position = glm::vec3(0.268f, 0.5f, 0);
  rotation = glm::vec3(0, 0, 0);
  m_view = glm::lookAt(position, vec3(0.0f, 0.0f, -1.0f),
                       vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A5::initLightSources()
{
}

//----------------------------------------------------------------------------------------
void A5::uploadCommonSceneUniforms()
{

  for (auto it = this->shaders.begin(); it != this->shaders.end(); it++)
  {
    std::string key = it->first;
    ShaderProgram *shader = it->second;
    shader->enable();

    if (key == "uvShader")
    {
      {
        //-- Set Perpsective matrix uniform for the scene:
        GLint location = shader->getUniformLocation("Perspective");
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
        CHECK_GL_ERRORS;

        //-- Set LightSource uniform for the scene:
        {
          int iter = std::min(4, (int)this->lights.size());
          GLint numLights = shader->getUniformLocation("numLights");
          glUniform1i(numLights, iter);

          for (int i = 0; i < iter; i++)
          {
            std::string colorStr = "light[" + std::to_string(i) + "].color";
            location = shader->getUniformLocation(colorStr.c_str());
            if (i < this->lights.size())
            {
              glUniform3fv(location, 1, value_ptr(this->lights[i]->lightInfo.color));
            }
            else
            {
              glm::vec3 defaultColor = glm::vec3(0.0);
              glUniform3fv(location, 1, value_ptr(defaultColor));
            }

            std::string intensityStr = "light[" + std::to_string(i) + "].intensity";
            location = shader->getUniformLocation(intensityStr.c_str());
            if (i < this->lights.size() && this->lights[i]->enabled == true)
            {
                glUniform1f(location, this->lights[i]->lightInfo.intensity);
            }
            else
            {
              float defaultIntensity = 0.0f;
              glUniform1f(location, defaultIntensity);
            }

            CHECK_GL_ERRORS;
          }
        }

        {
          location = shader->getUniformLocation("farPlane");
          glUniform1f(location, far);

          CHECK_GL_ERRORS;
        }

        {
          // Set uniforms
          GLint location = shader->getUniformLocation("toggleTexture");
          glUniform1i(location, this->toggleTexture);

          location = shader->getUniformLocation("toggleNormalMap");
          glUniform1i(location, this->toggleNormalMap);

          location = shader->getUniformLocation("toggleCelShading");
          glUniform1i(location, this->toggleCelShading);

          location = shader->getUniformLocation("toggleShadows");
          glUniform1i(location, this->toggleShadows);

        }

        //-- Set background light ambient intensity
        {
          location = shader->getUniformLocation("ambientIntensity");
          vec3 ambientIntensity(0.05f);
          glUniform3fv(location, 1, value_ptr(ambientIntensity));
          CHECK_GL_ERRORS;
        }

        {
          int iter = std::min(4, (int)this->lights.size());
          for (int i = 0; i < iter; i++)
          {
            std::string colorStr = "light[" + std::to_string(i) + "].position";
            location = shader->getUniformLocation(colorStr.c_str());
            if (i < this->lights.size())
            {
              glUniform3fv(location, 1, value_ptr(this->lights[i]->animPos));
            }
            else
            {
              glm::vec3 defaultPos = glm::vec3(0.0);
              glUniform3fv(location, 1, value_ptr(defaultPos));
            }
          }

          CHECK_GL_ERRORS;
        }
      }
    }
    else if (key == "noiseShader")
    {

      GLint location = shader->getUniformLocation("Perspective");
      glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
      CHECK_GL_ERRORS;

      
      int p[] = {
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
      7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
      190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
      203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
      20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27,
      166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
      220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
      63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
      200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
      186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118,
      126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
      189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163,
      70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
      98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,
      97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
      241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
      199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
      254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
      195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131,
      13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69,
      142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75,
      0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
      88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
      165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229,
      122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
      102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
      187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86,
      164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124,
      123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
      227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119,
      248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172,
      9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
      185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210,
      144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
      107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,
      115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114,
      67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
      };

      for (int i = 0; i < 512; i++) {
        std::string locStr = "p[" + to_string(i) + "]";
        location = shader->getUniformLocation(locStr.c_str());
        glUniform1i(location, p[i]);
      }
    }
    else
    {
      //std::cout << "Unknown shader" << std::endl;
    }
    shader->disable();
  }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A5::appLogic()
{
  // Place per frame, application logic here ...
  glfwGetCursorPos(this->m_window, &this->curMouseX, &this->curMouseY);

  float movementSpeed = 1.0f;
  glm::vec3 forward(rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]);
  glm::vec3 strafe(rotationMatrix[0][0], rotationMatrix[1][0], rotationMatrix[2][0]);

  float dx = 0;
  float dz = 0;

  if (this->upPressed)
  {
    dz += 2;
  }

  if (this->downPressed)
  {
    dz += -2;
  }

  if (this->leftPressed)
  {
    dx += -2;
  }

  if (this->rightPressed)
  {
    dx += 2;
  }
  glm::vec3 deltaPos = -dz * forward + dx * strafe;
  deltaPos.y = 0;

  glm::vec3 delta = (deltaPos)*movementSpeed * deltaTime;
  float limZ1 = -4.40f;
  float limZ2 = 0.65f;

  float limX1 = -4.0f;
  float limX2 = 4.0f;

  if (position.x + delta.x >= limX2 || position.x + delta.x <= limX1) {
    delta.x = 0;
  }

  if (position.z + delta.z >= limZ2 || position.z + delta.z <= limZ1) {
    delta.z = 0;
  }


  this->position += delta;
  this->positionMatrix = glm::translate(glm::mat4(), -glm::vec3(this->position));

  this->AnimLogic();
  uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the  method.
 */
void A5::guiLogic()
{
}

// Application Menu
void A5::resetPosition()
{
  this->initViewMatrix();
  this->viewTransformations = glm::mat4();
}

void A5::resetOrientation()
{
}
void A5::resetJoints()
{
  for (auto joint : this->joints)
  {
    joint->reset();
  }

  this->undoStack = std::stack<JointMoveCommand>();
  this->redoStack = std::stack<JointMoveCommand>();
  this->curJointMoveCommand.reset();
}
void A5::resetAll()
{
  this->resetPosition();
  this->resetOrientation();
  this->resetJoints();
}

void A5::switchInteractionMode(INTERACTION_MODE mode)
{
  this->interactionMode = mode;
  this->curIntMode = (int)this->interactionMode;
}

void A5::undo()
{
  if (this->undoStack.empty())
    return;

  JointMoveCommand command = this->undoStack.top();
  this->undoStack.pop();
  this->redoStack.emplace(command);
  command.undo();
}

void A5::redo()
{
  if (this->redoStack.empty())
    return;
  JointMoveCommand command = this->redoStack.top();
  this->redoStack.pop();
  this->undoStack.emplace(command);
  command.redo();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
void A5::updateShaderUniforms(
    const std::string &shaderName,
    const GeometryNode &node,
    const glm::mat4 &model)
{
  ShaderProgram *shader = this->shaders[shaderName];

  shader->enable();
  {
    if (shaderName == "uvShader")
    {

      //-- Set ModelView matrix:
      GLint location = shader->getUniformLocation("Model");

      glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(model));
      CHECK_GL_ERRORS;

      location = shader->getUniformLocation("View");

      glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(this->viewTransformations));
      CHECK_GL_ERRORS;

      glm::mat4 modelView = this->viewTransformations * model;

      //-- Set NormMatrix:
      location = shader->getUniformLocation("NormalMatrix");
      mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
      glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
      CHECK_GL_ERRORS;

      location = shader->getUniformLocation("material.kd");
      vec3 kd = node.material.kd;
      if (this->pickedJointChildren.find(node.m_nodeId) != this->pickedJointChildren.end())
      {
        kd = glm::vec3(1.0, 0.6181f, 0.1650f);
      }

      glUniform3fv(location, 1, value_ptr(kd));
      CHECK_GL_ERRORS;
      location = shader->getUniformLocation("material.ks");
      vec3 ks = node.material.ks;
      glUniform3fv(location, 1, value_ptr(ks));
      CHECK_GL_ERRORS;
      location = shader->getUniformLocation("material.shininess");
      glUniform1f(location, node.material.shininess);
      CHECK_GL_ERRORS;

      glBindTexture(GL_TEXTURE_2D, 0);

      GLint texLoc = shader->getUniformLocation("texture0");
      glUniform1i(texLoc, 0);

      texLoc = shader->getUniformLocation("normalMap");
      glUniform1i(texLoc, 1);

      texLoc = shader->getUniformLocation("depthMap");
      glUniform1i(texLoc, 2);

      bool hasTextureMap = node.material.hasTexture;
      location = shader->getUniformLocation("hasTextureMap");
      glUniform1i(location, hasTextureMap);

      if (hasTextureMap)
      {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, node.material.texture);
      }

      bool hasNormalMap = node.material.hasNormalMap;
      location = shader->getUniformLocation("hasNormalMap");
      glUniform1i(location, hasNormalMap);

      if (hasNormalMap)
      {
        glActiveTexture(GL_TEXTURE0 + 1); // Make sure to set uniform location
        glBindTexture(GL_TEXTURE_2D, node.material.normalMap);
      }

      // Shadows
      glActiveTexture(GL_TEXTURE0 + 2);
      glBindTexture(GL_TEXTURE_CUBE_MAP, shadowDepthCubemap);

      bool isCelShaded = node.material.celShaded;
      location = shader->getUniformLocation("isCelShaded");
      glUniform1i(location, isCelShaded);

      CHECK_GL_ERRORS;
    }
    else if (shaderName == "shadowDepthShader")
    {
      GLint location = shader->getUniformLocation("model");

      glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(model));
      CHECK_GL_ERRORS;
    }
    else if (shaderName == "noiseShader")
    {
      GLint location = shader->getUniformLocation("ModelView");
      glm::mat4 modelView = this->viewTransformations * model;
      glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));

      location = shader->getUniformLocation("absTime");
      glUniform1f(location, this->absTime);

      CHECK_GL_ERRORS;
    }
    else
    {
      //std::cout << "Unknown shader" << std::endl;
    }
  }

  shader->disable();
}

void A5::mainMenuLogic() {
    this->m_paused = true;
    static bool showMainMenuWindow(true);

    static bool firstRun(true);
    if (firstRun)
    {

      firstRun = false;
      this->appLogic();
      
    }
      ImGui::SetNextWindowPos(ImVec2(50, 50));
      ImGui::SetNextWindowPosCenter();
      ImGui::SetNextWindowSize(ImVec2(m_framebufferWidth,
                                      m_framebufferHeight));
    this->draw();

    ImGui::Begin("MAIN MENU", &showMainMenuWindow, ImVec2(m_framebufferWidth, m_framebufferHeight), 0.8f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(m_framebufferWidth * 0.5);
    ImGui::Text("Room - A OpenGL animation demo");
    ImGui::Text("Press W/A/S/D to move");
    ImGui::Text("Right click to look around");
    ImGui::Text("P/ESC to pause or adjust settings");
    ImGui::Text("SPACE to fast forward 5 seconds");

    if( ImGui::Button( "Continue" ) ) {
        this->m_paused = false;
        this->curState = SCENE_STATE::GAME;
        this->prevTime = std::chrono::steady_clock::now();
    }

  ///  this->m_paused = false;
   // this->curState = SCENE_STATE::GAME;
  //  this->prevTime = std::chrono::steady_clock::now();

    ImGui::End();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A5::draw()
{

    glEnable(GL_CULL_FACE);
    this->viewTransformations = rotationMatrix * positionMatrix;
    this->renderShadowSceneGraph();
    this->renderNormalSceneGraph();
}

void A5::renderShadowSceneGraph()
{

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  std::vector<glm::mat4> shadowTransforms;
  glm::vec3 lightPos = this->lights[0]->animPos; // TODO: Generalize to i.
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
  shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

  glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);

  ShaderProgram *shadowDepthShader = this->shaders["shadowDepthShader"];
  shadowDepthShader->enable();
  for (unsigned int i = 0; i < 6; ++i)
  {
    std::string shadowMatrixString = "shadowMatrices[" + std::to_string(i) + "]";
    GLuint matrixLoc = shadowDepthShader->getUniformLocation(shadowMatrixString.c_str());
    glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, value_ptr(shadowTransforms[i]));
  }

  GLuint location = shadowDepthShader->getUniformLocation("far_plane");
  glUniform1f(location, far);

  location = shadowDepthShader->getUniformLocation("lightPos");
  glUniform3fv(location, 1, value_ptr(lightPos));

  renderSceneGraph(*m_rootNode, glm::mat4(), "shadowDepthShader");

  shadowDepthShader->disable();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void A5::renderNormalSceneGraph()
{

  glViewport(0, 0, this->m_windowWidth, this->m_windowHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  renderSceneGraph(*m_rootNode, glm::mat4());

}

//----------------------------------------------------------------------------------------
void A5::renderSceneGraph(SceneNode &root, glm::mat4 model, std::string shaderOverride)
{
  if (root.enabled == false) return;

  glBindVertexArray(m_vao_meshData);


  mat4 nodeTrans = root.isAnimating == true ? root.getAnimMatrix() : root.trans;

  model = model * nodeTrans;

  switch (root.m_nodeType)
  {
  case NodeType::GeometryNode:
  {
    const GeometryNode *geometryNode = static_cast<const GeometryNode *>(&root);
    std::string shaderName = shaderOverride == "" ? geometryNode->material.shaderName : shaderOverride;
    if (this->shaders.find(shaderName) == this->shaders.end())
    {
      cout << "ERROR: Invalid shader name: " << shaderName << endl;
      return;
    }

    this->updateShaderUniforms(shaderName, *geometryNode, model);

    // Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
    BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];
    ShaderProgram *shader = this->shaders[shaderName];

    //-- Now render the mesh:
    shader->enable();

    //glDrawArrays(GL_TRIANGLES, 0, batchInfo.numIndices);
    glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
    shader->disable();
    break;
  }
  }

  if (root.particleSystem != nullptr && shaderOverride == "" && this->toggleParticleSystem)
  {
    glm::vec3 nodePos = glm::vec3(nodeTrans[3][0], nodeTrans[3][1], nodeTrans[3][2]);
    root.particleSystem->isSpawning = true;
    root.particleSystem->Update(this->deltaTime, nodePos, this->position, model);
    root.particleSystem->Draw();
  }

  for (SceneNode *node : root.children)
  {
    this->renderSceneGraph(*node, model, shaderOverride);
  }

  glBindVertexArray(0);
  CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A5::cleanup()
{
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A5::cursorEnterWindowEvent(
    int entered)
{
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A5::mouseMoveEvent(
    double xPos,
    double yPos)
{
  bool eventHandled(false);

  // Fill in with event handling code...

  if (!ImGui::IsMouseHoveringAnyWindow())
  {
    double deltaX = xPos - this->lastMouseX;
    double deltaY = yPos - this->lastMouseY;

    float movementX = static_cast<float>(deltaX * SCROLL_SPEED);
    float movementY = static_cast<float>(deltaY * SCROLL_SPEED);

    if (this->interactionMode == INTERACTION_MODE::POSITION_ORIENTATION)
    {
      if (this->leftClickActive)
      {
      }
      if (this->middleClickActive)
      {
      }
      if (this->rightClickActive && this->absTime > 0.05f) 
      {
        //  if (rotation.y + movementX >= -1.0f && rotation.y + movementX <= 1.0f ) {
        //camYAngle += movementX;
        rotation.y += movementX;
        //  }

        if (rotation.x + movementY >= -1.0f && rotation.x + movementY <= 0.5f)
        {
          rotation.x += movementY;
        }
        rotationMatrix = glm::rotate(glm::mat4(), rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(), rotation.y, glm::vec3(0, 1, 0));
      }
    }

    this->lastMouseX = xPos;
    this->lastMouseY = yPos;
  }

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A5::mouseButtonInputEvent(
    int button,
    int actions,
    int mods)
{
  bool eventHandled(false);

  if (!ImGui::IsMouseHoveringAnyWindow())
  {
    if (actions == GLFW_PRESS)
    {
      if (button == GLFW_MOUSE_BUTTON_LEFT)
      {
        this->leftClickActive = true;
      }
      else if (button == GLFW_MOUSE_BUTTON_RIGHT)
      {
        this->rightClickActive = true;
        if (this->interactionMode == INTERACTION_MODE::JOINTS && this->pickedJoints.size() > 0)
        {
          this->recordJointCommand();
        }
      }
      else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
      {
        this->middleClickActive = true;
        if (this->interactionMode == INTERACTION_MODE::JOINTS && this->pickedJoints.size() > 0)
        {
          this->recordJointCommand();
        }
      }

      eventHandled = true;
    }
    else if (actions == GLFW_RELEASE)
    {
      if (button == GLFW_MOUSE_BUTTON_LEFT)
      {
        this->leftClickActive = false;
      }
      else if (button == GLFW_MOUSE_BUTTON_RIGHT)
      {
        this->rightClickActive = false;
        if (this->interactionMode == INTERACTION_MODE::JOINTS)
        {
          this->saveJointCommand();
        }
      }
      else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
      {
        this->middleClickActive = false;
        if (this->interactionMode == INTERACTION_MODE::JOINTS)
        {
          this->saveJointCommand();
        }
      }
      eventHandled = true;
    }
  }

  // Fill in with event handling code...

  return eventHandled;
}

void A5::recordJointCommand()
{
  this->curJointMoveCommand.reset();
  for (unsigned int jointId : this->pickedJoints)
  {
    JointNode *joint = this->nodeToJoint[jointId];
    this->curJointMoveCommand.nodes.emplace_back(joint);
    this->curJointMoveCommand.oldX.emplace_back(joint->getCurXRot());
    this->curJointMoveCommand.oldY.emplace_back(joint->getCurYRot());
  }
}

void A5::saveJointCommand()
{
  for (int i = 0; i < this->curJointMoveCommand.nodes.size(); i++)
  {
    JointNode *joint = this->curJointMoveCommand.nodes[i];
    this->curJointMoveCommand.newX.emplace_back(joint->getCurXRot());
    this->curJointMoveCommand.newY.emplace_back(joint->getCurYRot());
  }
  this->undoStack.emplace(this->curJointMoveCommand);
  this->redoStack = std::stack<JointMoveCommand>();
  this->curJointMoveCommand.reset();
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A5::mouseScrollEvent(
    double xOffSet,
    double yOffSet)
{
  bool eventHandled(false);

  // Fill in with event handling code...

  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A5::windowResizeEvent(
    int width,
    int height)
{
  bool eventHandled(false);
  initPerspectiveMatrix();
  return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A5::keyInputEvent(
    int key,
    int action,
    int mods)
{
  bool eventHandled(false);

  if (action == GLFW_PRESS)
  {
    if (key == GLFW_KEY_M)
    {
      show_gui = !show_gui;
      eventHandled = true;
    }
    if (key == GLFW_KEY_L) {
        this->m_paused = false;
        this->curState = SCENE_STATE::GAME;
        this->prevTime = std::chrono::steady_clock::now();
   
    }
  }

  if (action == GLFW_PRESS)
  {
    if (key == GLFW_KEY_W)
    {
      this->upPressed = true;
    }
    else if (key == GLFW_KEY_S)
    {
      this->downPressed = true;
    }
    else if (key == GLFW_KEY_A)
    {
      this->leftPressed = true;
    }
    else if (key == GLFW_KEY_D)
    {
      this->rightPressed = true;
    }

    if (key == GLFW_KEY_SPACE) {
      this->absTime += 5;
    }

    if (key == GLFW_KEY_P || key == GLFW_KEY_ESCAPE)
    {
      this->m_paused = !this->m_paused;
      this->prevTime = std::chrono::steady_clock::now();
      eventHandled = true;

    }
  }
  else if (action == GLFW_RELEASE)
  {
    if (key == GLFW_KEY_W)
    {
      this->upPressed = false;
    }
    else if (key == GLFW_KEY_S)
    {
      this->downPressed = false;
    }
    else if (key == GLFW_KEY_A)
    {
      this->leftPressed = false;
    }
    else if (key == GLFW_KEY_D)
    {
      this->rightPressed = false;
    }
  }

  return eventHandled;
}

// Calculates rotation vector based on mouse motion over virtual trackball
// Source: trackball demo given on piazza by Michael Hardy
glm::vec3 A5::vCalcRotVec(float fNewX, float fNewY,
                          float fOldX, float fOldY,
                          float fDiameter)
{
  long nXOrigin, nYOrigin;
  float fNewVecX, fNewVecY, fNewVecZ, /* Vector corresponding to new mouse location */
      fOldVecX, fOldVecY, fOldVecZ,   /* Vector corresponding to old mouse location */
      fLength;

  /* Vector pointing from center of virtual trackball to
    * new mouse position
    */
  fNewVecX = fNewX * 2.0 / fDiameter;
  fNewVecY = fNewY * 2.0 / fDiameter;
  fNewVecZ = (1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY);

  /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
  if (fNewVecZ < 0.0)
  {
    fLength = sqrt(1.0 - fNewVecZ);
    fNewVecZ = 0.0;
    fNewVecX /= fLength;
    fNewVecY /= fLength;
  }
  else
  {
    fNewVecZ = sqrt(fNewVecZ);
  }
  /* Vector pointing from center of virtual trackball to
    * old mouse position
    */
  fOldVecX = fOldX * 2.0 / fDiameter;
  fOldVecY = fOldY * 2.0 / fDiameter;
  fOldVecZ = (1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY);

  /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
  if (fOldVecZ < 0.0)
  {
    fLength = sqrt(1.0 - fOldVecZ);
    fOldVecZ = 0.0;
    fOldVecX /= fLength;
    fOldVecY /= fLength;
  }
  else
  {
    fOldVecZ = sqrt(fOldVecZ);
  }

  /* Generate rotation vector by calculating cross product:
    * 
    * fOldVec x fNewVec.
    * 
    * The rotation vector is the axis of rotation
    * and is non-unit length since the length of a crossproduct
    * is related to the angle between fOldVec and fNewVec which we need
    * in order to perform the rotation.
    */
  float fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
  float fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
  float fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;

  return glm::vec3(fVecX, fVecY, fVecZ);
}

 void A5::pauseLogic() {
    this->draw();
    
    static bool showPauseWindow(true);
    if (showPauseWindow) {
      ImGui::SetNextWindowPosCenter();
      ImGui::SetNextWindowSize(ImVec2(m_framebufferWidth * 0.5,
                                      m_framebufferHeight * 0.5));
    }

    ImGui::Begin("PAUSED", &showPauseWindow, ImVec2(m_framebufferWidth * 0.5, m_framebufferHeight * 0.5), 0.5f, ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Paused");
    ImGui::Checkbox("Toggle Textures", &toggleTexture);
    ImGui::Checkbox("Toggle Normal maps", &toggleNormalMap);
    ImGui::Checkbox("Toggle Shadows", &toggleShadows);
    ImGui::Checkbox("Toggle Cel shading", &toggleCelShading);
    ImGui::Checkbox("Toggle Animation", &toggleAnimation);
    ImGui::Checkbox("Toggle Particle System", &toggleParticleSystem);
    ImGui::Text("Press P or ESC to continue...");

    ImGui::End();
 }

void A5::mapJoints(SceneNode &root)
{
  if (root.m_nodeType == NodeType::JointNode)
  {
    JointNode *joint = static_cast<JointNode *>(&root);
    this->nodeToJoint[root.m_nodeId] = joint;
    this->joints.emplace(joint);
    for (const SceneNode *node : root.children)
    {
      if (node->m_nodeType == NodeType::GeometryNode)
      {
        this->nodeToJoint[node->m_nodeId] = joint;
      }
    }
  }

  for (SceneNode *node : root.children)
  {
    this->mapJoints(*node);
  }
}

std::string A5::getAssetPath(const char *base)
{
  return getAssetFilePath(base);
}

void A5::InitPass(SceneNode &root)
{

  root.getAnimMetaData();

  for (AnimLoop * loop : root.animLoops) {
    loop->Init(root.frames);
  }
  
  for (Frame *frame : root.frames)
  {
    this->frames.emplace_back(frame);
  }

  if (root.particleSystem != nullptr)
  {
    root.particleSystem->Init(this->shaders["particleSystem"], &this->viewTransformations, &this->m_perpsective);
  }

  switch (root.m_nodeType)
  {
  case NodeType::GeometryNode:
  {
    break;
  }
  case NodeType::JointNode:
    break;
  case NodeType::LightNode:
    if (this->lights.size() == 4)
    {
      std::cout << "max lights = 4" << std::endl;
      break;
    }
    LightNode *lightNode = static_cast<LightNode *>(&root);
    this->lights.emplace_back(lightNode);
    break;
  }

  for (SceneNode *node : root.children)
  {
    this->InitPass(*node);
  }
}

void A5::PostInitPass()
{
  this->sortAnimFrames();
  this->prevTime = std::chrono::steady_clock::now();
}

void A5::sortAnimFrames()
{
  std::sort(this->frames.begin(), this->frames.end(), [](Frame *a, Frame *b) {
    return a->startTime < b->startTime;
  });
}


void A5::AnimLogic()
{
  if (!this->toggleAnimation || this->m_paused) {
    return;
  }

  cout << "Anim Time: " << this->absTime << endl;

  deltaTime = 0;
  auto elapsed = std::chrono::steady_clock::now() - this->prevTime;
  deltaTime = elapsed.count() / 1000000000.0f;
  this->absTime += deltaTime;
  vector<Frame *> deleteFrames;

  for (Frame *frame : this->frames)
  {
    SceneNode *node = frame->node;

    if (this->absTime >= frame->startTime)
    {

      float t = (this->absTime - frame->startTime) / (frame->endTime - frame->startTime);
      t = glm::clamp(t, 0.0f, 1.0f);

      switch (frame->fType)
      {
      case FrameType::TRANSLATE:
        node->animPos = glm::lerp(frame->transformDataFrom, frame->transformDataTo, t);
        break;
      case FrameType::SCALE:
        node->animScale = glm::lerp(frame->transformDataFrom, frame->transformDataTo, t);
        break;
      case FrameType::ROTATE_X:
        node->animRot.x = glm::lerp(frame->magDataFrom, frame->magDataTo, t);
        break;
      case FrameType::ROTATE_Y:
        node->animRot.y = glm::lerp(frame->magDataFrom, frame->magDataTo, t);
        break;
      case FrameType::ROTATE_Z:
        node->animRot.z = glm::lerp(frame->magDataFrom, frame->magDataTo, t);
        break;
      case FrameType::SOUND:
        if (frame->hasTriggered == false)
        {
          if (frame->boolData == true)
          {
            this->soundPlayer.PlayLoopedSound(getAssetPath(frame->strData.c_str()), frame->magDataFrom);
          }
          else
          {
            this->soundPlayer.PlaySoundOneShot(getAssetPath(frame->strData.c_str()), frame->magDataFrom);
          }
          frame->hasTriggered = true;
        }
        break;
      case FrameType::ENABLE_DISABLE:
       node->enabled = frame->boolData;
      break;
      case FrameType::STOP_SOUND:
        if (frame->hasTriggered == false) {
          frame->hasTriggered = true;
          this->soundPlayer.StopSound();
        }
      break;
      }
      node->isAnimating = true;
    }
    
    if (this->absTime > frame->endTime)
    {
      deleteFrames.emplace_back(frame);
    }
  }


  for (Frame *frame : deleteFrames)
  {
    SceneNode *node = frame->node;
    node->SetCurAnimLoop(this->absTime);

    auto globalFramesRef = std::find(this->frames.begin(), this->frames.end(), frame);
    this->frames.erase(globalFramesRef);

    AnimLoop * curLoop = node->curAnimLoop;
    
    if (curLoop != nullptr  && curLoop->inRange(absTime) == true && curLoop->inRange(frame->startTime, frame->endTime)) {
      curLoop->disabledFrames.emplace_back(frame);

       float deltaOffset = curLoop->animDuration;
       if (curLoop->inRange(frame->startTime + deltaOffset, frame->endTime + deltaOffset)) {
        frame->startTime += deltaOffset;
        frame->endTime += deltaOffset;
        frame->hasTriggered = false;
        this->frames.emplace_back(frame);
        this->sortAnimFrames();
       }

    }
  }

  this->prevTime = std::chrono::steady_clock::now();
}
