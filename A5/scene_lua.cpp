// Winter 2019

//
// CS488 - Introduction to Computer Graphics
//
// scene_lua.cpp
//
// Everything that's needed to parse a scene file using Lua.
// You don't necessarily have to understand exactly everything that
// goes on here, although it will be useful to have a reasonable idea
// if you wish to add new commands to the scene format.
//
// Lua interfaces with C/C++ using a special stack. Everytime you want
// to get something from lua, or pass something back to lua (e.g. a
// return value), you need to use this stack. Thus, most of the lua_
// and luaL_ functions actually manipulate the stack. All the
// functions beginning with "lua_" are part of the Lua C API itself,
// whereas the "luaL_" functions belong to a library of useful
// functions on top of that called lauxlib.
//
// This file consists of a bunch of C function declarations which
// implement functions callable from Lua. There are also two tables
// used to set up the interface between Lua and these functions, and
// the main "driver" function, import_lua, which calls the lua
// interpreter and sets up all the state.
//
// Note that each of the function declarations follow the same format:
// they take as their only argument the current state of the lua
// interpreter, and return the number of values returned back to lua.
//
// For more information see the book "Programming In Lua," available
// online at http://www.lua.org/pil/, and of course the Lua reference
// manual at http://www.lua.org/manual/5.0/.
//
// http://lua-users.org/wiki/LauxLibDocumentation provides a useful
// documentation of the "lauxlib" functions (beginning with luaL_).
//
// -- University of Waterloo Computer Graphics Lab 2005

#include "scene_lua.hpp"
#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdio>
#include "lua488.hpp"
#include "JointNode.hpp"
#include "GeometryNode.hpp"
#include "Frame.hpp"
#include "ParticleSystem.hpp"
#include "LightNode.hpp"

// Uncomment the following line to enable debugging messages
//#define GRLUA_ENABLE_DEBUG

#ifdef GRLUA_ENABLE_DEBUG
#define GRLUA_DEBUG(x)           \
  do                             \
  {                              \
    std::cerr << x << std::endl; \
  } while (0)
#define GRLUA_DEBUG_CALL                    \
  do                                        \
  {                                         \
    std::cerr << __FUNCTION__ << std::endl; \
  } while (0)
#else
#define GRLUA_DEBUG(x) \
  do                   \
  {                    \
  } while (0)
#define GRLUA_DEBUG_CALL \
  do                     \
  {                      \
  } while (0)
#endif

// You may wonder, for the following types, why we use special "_ud"
// types instead of, for example, just allocating SceneNodes directly
// from lua. Part of the answer is that Lua is a C api. It doesn't
// call any constructors or destructors for you, so it's easier if we
// let it just allocate a pointer for the node, and handle
// allocation/deallocation of the node itself. Another (perhaps more
// important) reason is that we will want SceneNodes to stick around
// even after lua is done with them, after all, we want to pass them
// back to the program. If we let Lua allocate SceneNodes directly,
// we'd lose them all when we are done parsing the script. This way,
// we can easily keep around the data, all we lose is the extra
// pointers to it.

// The "userdata" type for a node. Objects of this type will be
// allocated by Lua to represent nodes.
struct gr_node_ud
{
  SceneNode *node;
};

// The "userdata" type for a material. Objects of this type will be
// allocated by Lua to represent materials.
struct gr_material_ud
{
  Material *material;
};

struct gr_frame_ud
{
  Frame *frame;
};

struct gr_particle_system_ud
{
  ParticleSystem *particleSystem;
};

// Create a node
extern "C" int gr_node_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *data = (gr_node_ud *)lua_newuserdata(L, sizeof(gr_node_ud));
  data->node = 0;

  const char *name = luaL_checkstring(L, 1);
  data->node = new SceneNode(name);

  luaL_getmetatable(L, "gr.node");
  lua_setmetatable(L, -2);

  return 1;
}

// Create a joint node
extern "C" int gr_joint_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *data = (gr_node_ud *)lua_newuserdata(L, sizeof(gr_node_ud));
  data->node = 0;

  const char *name = luaL_checkstring(L, 1);
  JointNode *node = new JointNode(name);

  luaL_checktype(L, 2, LUA_TTABLE);

  luaL_argcheck(L, luaL_len(L, 2) == 3, 2, "Three-tuple expected");

  luaL_checktype(L, 3, LUA_TTABLE);

  luaL_argcheck(L, luaL_len(L, 3) == 3, 3, "Three-tuple expected");

  double x[3], y[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, 2, i);
    x[i - 1] = luaL_checknumber(L, -1);
    lua_rawgeti(L, 3, i);
    y[i - 1] = luaL_checknumber(L, -1);
    lua_pop(L, 2);
  }

  node->set_joint_x(x[0], x[1], x[2]);
  node->set_joint_y(y[0], y[1], y[2]);

  data->node = node;

  luaL_getmetatable(L, "gr.node");
  lua_setmetatable(L, -2);

  return 1;
}
extern "C" int gr_mesh_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *data = (gr_node_ud *)lua_newuserdata(L, sizeof(gr_node_ud));
  data->node = 0;

  const char *meshId = luaL_checkstring(L, 1);
  const char *name = luaL_checkstring(L, 2);
  data->node = new GeometryNode(meshId, name);

  luaL_getmetatable(L, "gr.node");
  lua_setmetatable(L, -2);

  return 1;
}

extern "C" int gr_light_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *data = (gr_node_ud *)lua_newuserdata(L, sizeof(gr_node_ud));
  data->node = 0;
  
  int index = 1;
  const char *name = luaL_checkstring(L, index);
   
  index++;

  double color[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, index, i);
    color[i - 1] = luaL_checknumber(L, -1);
    lua_pop(L, 1);
  }

  index++;
  float intensity = luaL_checknumber(L, index);
  LightNode * light = new LightNode(name);
  light->lightInfo.color = glm::vec3(color[0], color[1], color[2]);
  light->lightInfo.intensity = intensity;
  data->node = light;

  luaL_getmetatable(L, "gr.node");
  lua_setmetatable(L, -2);

  return 1;
}


// Create a material
extern "C" int gr_material_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_material_ud *data = (gr_material_ud *)lua_newuserdata(L, sizeof(gr_material_ud));
  data->material = 0;

  luaL_checktype(L, 1, LUA_TTABLE);

  luaL_argcheck(L, luaL_len(L, 1) == 3, 1, "Three-tuple expected");

  luaL_checktype(L, 2, LUA_TTABLE);

  luaL_argcheck(L, luaL_len(L, 2) == 3, 2, "Three-tuple expected");

  luaL_checktype(L, 3, LUA_TNUMBER);

  double kd[3], ks[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, 1, i);
    kd[i - 1] = luaL_checknumber(L, -1);
    lua_rawgeti(L, 2, i);
    ks[i - 1] = luaL_checknumber(L, -1);
    lua_pop(L, 2);
  }
  double shininess = luaL_checknumber(L, 3);

  data->material = new Material();
  for (int i(0); i < 3; ++i)
  {
    data->material->kd[i] = kd[i];
    data->material->ks[i] = ks[i];
  }
  data->material->shininess = shininess;

  luaL_newmetatable(L, "gr.material");
  lua_setmetatable(L, -2);

  return 1;
}

// Standard material
// Create a material
extern "C" int gr_standard_material_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_material_ud *data = (gr_material_ud *)lua_newuserdata(L, sizeof(gr_material_ud));
  data->material = 0;

  int index = 1;

  const char *shaderString = luaL_checkstring(L, index);
  luaL_argcheck(L, shaderString, index, "shader string expected");

  index++;

  const char *textureString = luaL_checkstring(L, index);
  luaL_argcheck(L, textureString, index, "texture string expected");

  index++;

  const char *bumpMapFileName = luaL_checkstring(L, index);
  luaL_argcheck(L, bumpMapFileName, index, "bump map string expected");

  index++;

  luaL_checktype(L, index, LUA_TTABLE);
  luaL_argcheck(L, luaL_len(L, index) == 3, 3, "Three-tuple expected");
  double kd[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, index, i);
    kd[i - 1] = luaL_checknumber(L, -1);
    lua_pop(L, 1);
  }

  index++;

  double ks[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, index, i);
    ks[i - 1] = luaL_checknumber(L, -1);
    lua_pop(L, 1);
  }

  index++;

  luaL_checktype(L, index, LUA_TNUMBER);

  double shininess = luaL_checknumber(L, index);

  index++;

  data->material = new Material();
  std::string realShaderString = std::string(shaderString);


  if (realShaderString == "toonShader"){
    realShaderString = "uvShader";
    data->material->celShaded = true;
  }

  data->material->shaderName = realShaderString;
  for (int i(0); i < 3; ++i)
  {
    data->material->kd[i] = kd[i];
    data->material->ks[i] = ks[i];
  }

  data->material->shininess = shininess;
  data->material->SetTexture(textureString);
  data->material->SetNormalMap(bumpMapFileName);
  

  luaL_newmetatable(L, "gr.standard_material");
  lua_setmetatable(L, -2);
  return 1;
}

extern "C" int gr_frame_translate_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  int index = 1;

  luaL_checktype(L, index, LUA_TNUMBER);
  float startTime = luaL_checknumber(L, index);

  index++;

  luaL_checktype(L, index, LUA_TNUMBER);
  float endTime = luaL_checknumber(L, index);

  index++;

  luaL_checktype(L, index, LUA_TTABLE);
  luaL_argcheck(L, luaL_len(L, index) == 3, 3, "Three-tuple expected");

  float transStart[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, index, i);
    transStart[i - 1] = luaL_checknumber(L, -1);
    lua_pop(L, 1);
  }

  index++;

  float transEnd[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, index, i);
    transEnd[i - 1] = luaL_checknumber(L, -1);
  }

  gr_frame_ud *data = (gr_frame_ud *)lua_newuserdata(L, sizeof(gr_frame_ud));
  data->frame = 0;

  glm::vec3 transStartVec = glm::vec3(transStart[0], transStart[1], transStart[2]);
  glm::vec3 transEndVec = glm::vec3(transEnd[0], transEnd[1], transEnd[2]);

  Frame *newFrame = new Frame();
  newFrame->SetFrameType(FrameType::TRANSLATE);
  newFrame->SetStartEndTime(startTime, endTime);
  newFrame->SetTransformationData(transStartVec, transEndVec);
  data->frame = newFrame;

  luaL_newmetatable(L, "gr.frame");
  lua_setmetatable(L, -2);
  return 1;
}



extern "C" int gr_frame_scale_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  int index = 1;

  luaL_checktype(L, index, LUA_TNUMBER);
  float startTime = luaL_checknumber(L, index);

  index++;

  luaL_checktype(L, index, LUA_TNUMBER);
  float endTime = luaL_checknumber(L, index);

  index++;

  luaL_checktype(L, index, LUA_TTABLE);
  luaL_argcheck(L, luaL_len(L, index) == 3, 3, "Three-tuple expected");

  float transStart[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, index, i);
    transStart[i - 1] = luaL_checknumber(L, -1);
    lua_pop(L, 1);
  }

  index++;

  float transEnd[3];
  for (int i = 1; i <= 3; i++)
  {
    lua_rawgeti(L, index, i);
    transEnd[i - 1] = luaL_checknumber(L, -1);
  }

  gr_frame_ud *data = (gr_frame_ud *)lua_newuserdata(L, sizeof(gr_frame_ud));
  data->frame = 0;

  glm::vec3 transStartVec = glm::vec3(transStart[0], transStart[1], transStart[2]);
  glm::vec3 transEndVec = glm::vec3(transEnd[0], transEnd[1], transEnd[2]);

  Frame *newFrame = new Frame();
  newFrame->SetFrameType(FrameType::SCALE);
  newFrame->SetStartEndTime(startTime, endTime);
  newFrame->SetTransformationData(transStartVec, transEndVec);
  data->frame = newFrame;

  luaL_newmetatable(L, "gr.frame");
  lua_setmetatable(L, -2);
  return 1;
}

extern "C" int gr_frame_rotate_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  int index = 1;

  luaL_checktype(L, index, LUA_TNUMBER);
  float startTime = luaL_checknumber(L, index);

  index++;

  luaL_checktype(L, index, LUA_TNUMBER);
  float endTime = luaL_checknumber(L, index);

  index++;

  const char *axisString = luaL_checkstring(L, index);

  luaL_argcheck(L, axisString && std::strlen(axisString) == 1, index, "Single character expected");
  char axis = std::tolower(axisString[0]);

  luaL_argcheck(L, axis >= 'x' && axis <= 'z', index, "Axis must be x, y or z");

  index++;
  luaL_checktype(L, index, LUA_TNUMBER);
  float angleStart = luaL_checknumber(L, index);

  index++;

  luaL_checktype(L, index, LUA_TNUMBER);
  float angleEnd = luaL_checknumber(L, index);

  index++;

  gr_frame_ud *data = (gr_frame_ud *)lua_newuserdata(L, sizeof(gr_frame_ud));
  data->frame = 0;

  Frame *newFrame = new Frame();
  if (axis == 'x')
  {
    newFrame->SetFrameType(FrameType::ROTATE_X);
  }
  else if (axis == 'y')
  {
    newFrame->SetFrameType(FrameType::ROTATE_Y);
  }
  else
  {
    newFrame->SetFrameType(FrameType::ROTATE_Z);
  }

  newFrame->SetStartEndTime(startTime, endTime);
  newFrame->SetMagData(angleStart, angleEnd);

  data->frame = newFrame;

  luaL_newmetatable(L, "gr.frame");
  lua_setmetatable(L, -2);
  return 1;
}

extern "C" int gr_frame_sound_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  int index = 1;

  luaL_checktype(L, index, LUA_TNUMBER);
  float startTime = luaL_checknumber(L, index);

  index++;

  const char *fileNameStr = luaL_checkstring(L, index);

  index++;

  float volume = luaL_checknumber(L, index);

  index++;

  int loop = luaL_checkinteger(L, index);
  bool loopBool = loop == 1 ? true : false;

  gr_frame_ud *data = (gr_frame_ud *)lua_newuserdata(L, sizeof(gr_frame_ud));
  data->frame = 0;

  Frame *newFrame = new Frame();
  newFrame->SetFrameType(FrameType::SOUND);
  newFrame->SetStartEndTime(startTime, startTime + 1.0f);
  newFrame->SetStrData(fileNameStr);
  newFrame->SetBoolData(loopBool);
  newFrame->SetMagData(volume, volume + 1);
  data->frame = newFrame;

  luaL_newmetatable(L, "gr.frame");
  lua_setmetatable(L, -2);
  return 1;
}

extern "C" int gr_frame_enabled_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  int index = 1;

  luaL_checktype(L, index, LUA_TNUMBER);
  float startTime = luaL_checknumber(L, index);

  index++;

  int enabled = luaL_checkinteger(L, index);
  bool enabledBool = enabled == 1 ? true : false;

  gr_frame_ud *data = (gr_frame_ud *)lua_newuserdata(L, sizeof(gr_frame_ud));
  data->frame = 0;

  Frame *newFrame = new Frame();
  newFrame->SetFrameType(FrameType::ENABLE_DISABLE);
  newFrame->SetStartEndTime(startTime, startTime + 1.0f);
  newFrame->SetBoolData(enabledBool);
  data->frame = newFrame;

  luaL_newmetatable(L, "gr.frame");
  lua_setmetatable(L, -2);
  return 1;
}


extern "C" int gr_frame_stop_sound_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  int index = 1;

  luaL_checktype(L, index, LUA_TNUMBER);
  float startTime = luaL_checknumber(L, index);

  gr_frame_ud *data = (gr_frame_ud *)lua_newuserdata(L, sizeof(gr_frame_ud));
  data->frame = 0;

  Frame *newFrame = new Frame();
  newFrame->SetFrameType(FrameType::STOP_SOUND);
  newFrame->SetStartEndTime(startTime, startTime + 1.0f);
  data->frame = newFrame;

  luaL_newmetatable(L, "gr.frame");
  lua_setmetatable(L, -2);
  return 1;
}


extern "C" int gr_particle_system_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  int index = 1;

  //luaL_checktype(L, index, LUA_TNUMBER);
  // float startTime = luaL_checknumber(L, index);
  
  const char *particleSystemName = luaL_checkstring(L, index); // Can be used for "special" PS later...

  index++;
  const char *texturePath = luaL_checkstring(L, index);

  index++;

  gr_particle_system_ud *data = (gr_particle_system_ud *)lua_newuserdata(L, sizeof(gr_particle_system_ud));

  ParticleSystem *newPS = new ParticleSystem();
  // TODO: Add parameters.
  data->particleSystem = newPS;
  data->particleSystem->name = particleSystemName;
  data->particleSystem->SetTexture(texturePath);

  luaL_newmetatable(L, "gr.particle_system");
  lua_setmetatable(L, -2);
  return 1;
}

// Add a child to a node
extern "C" int gr_node_add_child_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  SceneNode *self = selfdata->node;

  gr_node_ud *childdata = (gr_node_ud *)luaL_checkudata(L, 2, "gr.node");
  luaL_argcheck(L, childdata != 0, 2, "Node expected");

  SceneNode *child = childdata->node;

  self->add_child(child);

  return 0;
}

// Set a node's material
extern "C" int gr_node_set_material_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  GeometryNode *self = dynamic_cast<GeometryNode *>(selfdata->node);

  luaL_argcheck(L, self != 0, 1, "Geometry node expected");

  gr_material_ud *matdata = (gr_material_ud *)luaL_checkudata(L, 2, "gr.material");
  luaL_argcheck(L, matdata != 0, 2, "Material expected");

  Material *material = matdata->material;
  self->material.kd = material->kd;
  self->material.ks = material->ks;
  self->material.shininess = material->shininess;

  return 0;
}

extern "C" int gr_node_set_frame_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  SceneNode *self = dynamic_cast<SceneNode *>(selfdata->node);

  luaL_argcheck(L, self != 0, 1, "Geometry node expected");

  gr_frame_ud *frameData = (gr_frame_ud *)luaL_checkudata(L, 2, "gr.frame");
  luaL_argcheck(L, frameData != 0, 2, "Material expected");

  Frame *frame = new Frame(frameData->frame);

  if (frame->fType == FrameType::ENABLE_DISABLE && frame->startTime == 0) {
    self->enabled = frame->boolData;
  } 

  frame->SetSceneNode(self);
  self->frames.emplace_back(frame);

  return 0;
}

extern "C" int gr_node_set_particle_system_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  SceneNode *self = dynamic_cast<SceneNode *>(selfdata->node);

  luaL_argcheck(L, self != 0, 1, "Geometry node expected");

  gr_particle_system_ud *frameData = (gr_particle_system_ud *)luaL_checkudata(L, 2, "gr.particle_system");
  luaL_argcheck(L, frameData != 0, 2, "Material expected");

  ParticleSystem *ps = frameData->particleSystem;
  self->particleSystem = ps;

  return 0;
}

extern "C" int gr_node_set_standard_material_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  GeometryNode *self = dynamic_cast<GeometryNode *>(selfdata->node);

  luaL_argcheck(L, self != 0, 1, "Geometry node expected");

  gr_material_ud *matdata = (gr_material_ud *)luaL_checkudata(L, 2, "gr.standard_material");
  luaL_argcheck(L, matdata != 0, 2, "Material expected");

  Material *material = matdata->material;
  self->material = *material;

  return 0;
}

// Add a scaling transformation to a node.
extern "C" int gr_node_scale_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  SceneNode *self = selfdata->node;

  double values[3];

  for (int i = 0; i < 3; i++)
  {
    values[i] = luaL_checknumber(L, i + 2);
  }

  self->scale(glm::vec3(values[0], values[1], values[2]));

  return 0;
}

// Add a translation to a node.
extern "C" int gr_node_translate_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  SceneNode *self = selfdata->node;

  double values[3];

  for (int i = 0; i < 3; i++)
  {
    values[i] = luaL_checknumber(L, i + 2);
  }

  self->translate(glm::vec3(values[0], values[1], values[2]));

  return 0;
}

extern "C" int gr_node_loop_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  SceneNode *self = selfdata->node;

  int index = 2;
  int startTime = luaL_checknumber(L, index);

  index++;

  int endTime = luaL_checknumber(L, index);

  AnimLoop * animLoop = new AnimLoop(startTime, endTime);
  self->animLoops.emplace_back(animLoop);

  return 0;
}

// Rotate a node.
extern "C" int gr_node_rotate_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *selfdata = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, selfdata != 0, 1, "Node expected");

  SceneNode *self = selfdata->node;

  const char *axisString = luaL_checkstring(L, 2);

  luaL_argcheck(L, axisString && std::strlen(axisString) == 1, 2, "Single character expected");
  char axis = std::tolower(axisString[0]);

  luaL_argcheck(L, axis >= 'x' && axis <= 'z', 2, "Axis must be x, y or z");

  double angle = luaL_checknumber(L, 3);

  self->rotate(axis, angle);

  return 0;
}

// Garbage collection function for lua.
extern "C" int gr_node_gc_cmd(lua_State *L)
{
  GRLUA_DEBUG_CALL;

  gr_node_ud *data = (gr_node_ud *)luaL_checkudata(L, 1, "gr.node");
  luaL_argcheck(L, data != 0, 1, "Node expected");

  // Note that we don't delete the node here. This is because we still
  // want the scene to be around when we close the lua interpreter,
  // but at that point everything will be garbage collected.
  //
  // If data->node happened to be a reference-counted pointer, this
  // will in fact just decrease lua's reference to it, so it's not a
  // bad thing to include this line.
  data->node = 0;

  return 0;
}

// This is where all the "global" functions in our library are
// declared.
// If you want to add a new non-member function, add it here.
static const luaL_Reg grlib_functions[] = {
    {"node", gr_node_cmd},
    {"joint", gr_joint_cmd},
    {"mesh", gr_mesh_cmd},
    {"light", gr_light_cmd},
    {"material", gr_material_cmd},
    {"standard_material", gr_standard_material_cmd},
    {"frame_translate", gr_frame_translate_cmd},
    {"frame_rotate", gr_frame_rotate_cmd},
    {"frame_scale", gr_frame_scale_cmd},
    {"frame_enabled", gr_frame_enabled_cmd},
    {"particle_system", gr_particle_system_cmd},
    {"frame_sound", gr_frame_sound_cmd},
    {"frame_stop_sound", gr_frame_stop_sound_cmd},
    {0, 0}};

// This is where all the member functions for "gr.node" objects are
// declared. Since all the other objects (e.g. materials) are so
// simple, we only really need to make member functions for nodes.
//
// If you want to add a new member function for gr.node, add it
// here.
//
// We could have used inheritance in lua to match the inheritance
// between different node types, but it's easier to just give all
// nodes the same Lua type and then do any additional type checking in
// the appropriate member functions (e.g. gr_node_set_material_cmd
// ensures that the node is a GeometryNode, see above).
static const luaL_Reg grlib_node_methods[] = {
    {"__gc", gr_node_gc_cmd},
    {"add_child", gr_node_add_child_cmd},
    {"set_material", gr_node_set_material_cmd},
    {"set_standard_material", gr_node_set_standard_material_cmd},
    {"scale", gr_node_scale_cmd},
    {"rotate", gr_node_rotate_cmd},
    {"translate", gr_node_translate_cmd},
    {"set_frame", gr_node_set_frame_cmd},
    {"set_particle_system", gr_node_set_particle_system_cmd},
    {"anim_loop", gr_node_loop_cmd},
    {0, 0}};

// This function calls the lua interpreter to do the actual importing
SceneNode *import_lua(const std::string &filename)
{
  GRLUA_DEBUG("Importing scene from " << filename);

  // Start a lua interpreter
  lua_State *L = luaL_newstate();

  GRLUA_DEBUG("Loading base libraries");

  // Load some base library
  luaL_openlibs(L);

  GRLUA_DEBUG("Setting up our functions");

  // Set up the metatable for gr.node
  luaL_newmetatable(L, "gr.node");
  lua_pushstring(L, "__index");
  lua_pushvalue(L, -2);
  lua_settable(L, -3);

  // Load the gr.node methods

  luaL_setfuncs(L, grlib_node_methods, 0);

  // Load the gr functions
  luaL_setfuncs(L, grlib_functions, 0);
  lua_setglobal(L, "gr");

  GRLUA_DEBUG("Parsing the scene");
  // Now parse the actual scene
  if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 1, 0))
  {
    std::cerr << "Error loading " << filename << ": " << lua_tostring(L, -1) << std::endl;
    return 0;
  }

  GRLUA_DEBUG("Getting back the node");

  // Pull the returned node off the stack
  gr_node_ud *data = (gr_node_ud *)luaL_checkudata(L, -1, "gr.node");
  if (!data)
  {
    std::cerr << "Error loading " << filename << ": Must return the root node." << std::endl;
    return 0;
  }

  // Store it
  SceneNode *node = data->node;

  GRLUA_DEBUG("Closing the interpreter");

  // Close the interpreter, free up any resources not needed
  lua_close(L);

  // And return the node
  return node;
}
