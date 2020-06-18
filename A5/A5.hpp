// Winter 2019

#pragma once

#include "CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "MyMeshConsolidator.hpp"

#include "SceneNode.hpp"

#include <glm/glm.hpp>
#include <memory>

#include "GeometryNode.hpp"
#include "JointNode.hpp"
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <vector>
#include "Frame.hpp"
#include <chrono>
#include <string>
#include "SoundPlayer.hpp"
#include "LightNode.hpp"
#include <algorithm>

enum INTERACTION_MODE
{
  POSITION_ORIENTATION = 0,
  JOINTS = 1
};

struct JointMoveCommand
{
  std::vector<JointNode *> nodes;
  std::vector<double> oldX;
  std::vector<double> oldY;
  std::vector<double> newX;
  std::vector<double> newY;

  void undo()
  {
    for (int i = 0; i < nodes.size(); i++)
    {
      this->nodes[i]->setRotXY(oldX[i], oldY[i]);
    }
  }

  void redo()
  {
    for (int i = 0; i < nodes.size(); i++)
    {
      this->nodes[i]->setRotXY(newX[i], newY[i]);
    }
  }

  void reset()
  {
    this->nodes.clear();
    this->oldX.clear();
    this->newX.clear();
    this->newY.clear();
    this->oldY.clear();
  }
};



class A5 : public CS488Window
{
public:
  A5(const std::string &luaSceneFile);
  virtual ~A5();
  static std::string getAssetPath(const char * base);

protected:
  virtual void init() override;
  virtual void appLogic() override;
  virtual void guiLogic() override;
  virtual void draw() override;
  virtual void cleanup() override;
  virtual void mainMenuLogic() override;
  virtual void pauseLogic() override;

  //-- Virtual callback methods
  virtual bool cursorEnterWindowEvent(int entered) override;
  virtual bool mouseMoveEvent(double xPos, double yPos) override;
  virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
  virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
  virtual bool windowResizeEvent(int width, int height) override;
  virtual bool keyInputEvent(int key, int action, int mods) override;

  //-- One time initialization methods:
  void processLuaSceneFile(const std::string &filename);
  void createShaderProgram();
  void enableVertexShaderInputSlots();
  void uploadVertexDataToVbos(const MeshConsolidator &meshConsolidator);
  void mapVboDataToVertexShaderInputLocations();
  void initViewMatrix();
  void initLightSources();

  void initPerspectiveMatrix();
  void uploadCommonSceneUniforms();
  void renderSceneGraph(SceneNode &node, glm::mat4 modelView, std::string shaderOverride = "");

  glm::mat4 m_perpsective;
  glm::mat4 m_view;

  //-- GL resources for mesh geometry data:
  GLuint m_vao_meshData;
  GLuint m_vbo_vertexPositions;
  GLuint m_vbo_vertexNormals;
  GLuint m_vbo_vertexUv;
  GLuint m_vbo_vertexTangent;
  GLuint m_vbo_vertexBitangent;

  GLint m_positionAttribLocation;
  GLint m_normalAttribLocation;
  GLint m_uvAttribLocation;
  GLint m_tangentAttribLocation;
  GLint m_bitangentAttribLocation;

  std::unordered_map<std::string, ShaderProgram *> shaders;


  // BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
  // object. Each BatchInfo object contains an index offset and the number of indices
  // required to render the mesh with identifier MeshId.
  BatchInfoMap m_batchInfoMap;

  std::string m_luaSceneFile;

  std::shared_ptr<SceneNode> m_rootNode;

  // Menus:
  // Application Menu functions:
  void resetPosition();
  void resetOrientation();
  void resetJoints();
  void resetAll();

  // Edit menu items:
  void undo();
  void redo();

  // Options menu
  bool toggleCircle;
  bool toggleZBuffer = true;
  bool toggleBackfaceCulling;
  bool toggleFrontfaceCulling;
  
  // TODO: implement this functionality.
  bool toggleTexture = true;
  bool toggleNormalMap = true;
  bool toggleShadows = true;
  bool toggleCelShading = true;
  bool toggleAnimation = true;
  bool toggleParticleSystem = true;

  // Interaction mode:
  INTERACTION_MODE interactionMode = INTERACTION_MODE::POSITION_ORIENTATION;
  int curIntMode;

  void switchInteractionMode(INTERACTION_MODE mode);

  // Mouse event stuff:
  bool leftClickActive;
  bool rightClickActive;
  bool middleClickActive;

  double lastMouseX;
  double lastMouseY;
  double curMouseX;
  double curMouseY;

  // Geometric Transformations:
  glm::mat4 viewTransformations;

  // Material stuff:
  void updateShaderUniforms(
      const std::string &shaderName,
      const GeometryNode &node,
      const glm::mat4 &viewMatrix);

  // Rotation stuff:
  glm::vec3 vCalcRotVec(float fNewX, float fNewY,
                        float fOldX, float fOldY,
                        float fDiameter);

  // Picking/Joints stuff:
  void mapJoints(SceneNode &root);
  std::unordered_set<unsigned int> pickedJointChildren;
  std::unordered_set<unsigned int> pickedJoints;
  std::unordered_map<unsigned int, JointNode *> nodeToJoint;
  std::unordered_set<JointNode *> joints;
  bool isPicking;

  // Undo/Redo:
  JointMoveCommand curJointMoveCommand;
  std::stack<JointMoveCommand> undoStack;
  std::stack<JointMoveCommand> redoStack;
  void recordJointCommand();
  void saveJointCommand();

  bool upPressed = false;
  bool downPressed = false;
  bool leftPressed = false;
  bool rightPressed = false;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::mat4 positionMatrix;
  glm::mat4 rotationMatrix;

  void InitPass(SceneNode &node);
  void PostInitPass();
  void AnimLogic();
  void sortAnimFrames();
  
  bool testFlag = false;
  
  std::vector<Frame *> frames;
  
  std::chrono::steady_clock::time_point prevTime;
  float absTime = 0;
  float deltaTime = 0;

  ShaderProgram shadowDepthShader;
  GLuint shadowDepthMapFBO;
  GLuint shadowDepthCubemap;

  void initShadowDepthShader();

  glm::mat4 shadowProj;
  unsigned int SHADOWMAP_WIDTH = 1024;
  unsigned int SHADOWMAP_HEIGHT = 1024;

  void renderShadowSceneGraph();
  void renderNormalSceneGraph();


  float near = 0.1f;
  float far = 100.0f;

  SoundPlayer soundPlayer;

  void initShaderProgram(std::string name, std::string vert, std::string frag, std::string geo = "");

  std::vector<LightNode *> lights;
};
