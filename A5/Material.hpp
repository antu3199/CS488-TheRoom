// Winter 2019

#pragma once

#include <glm/glm.hpp>
#include "cs488-framework/OpenGLImport.hpp"
#include <string>
#include "Image.hpp"


// Material surface properties to be used as input into a local illumination model
// (e.g. the Phong Reflection Model).
class Material {
  public:
    Material()
        : kd(glm::vec3(0.0f)),
          ks(glm::vec3(0.0f)),
          shininess(0.0f) { }
    ~Material();

    // Diffuse reflection coefficient
    glm::vec3 kd;

    // Specular reflection coefficient
    glm::vec3 ks;

    // Material shininess constant.  Larger positive values model surfaces that
    // are smoother or mirror-like.  Smaller positive values model rougher surfaces.
    float shininess;
    GLuint texture;
    GLuint normalMap;
    
    bool hasTexture = false;
    bool hasNormalMap = false;
    
    void SetTexture(std::string path);
    void SetNormalMap(std::string path);
    std::string shaderName;
    bool celShaded = false;

  private:
    GLuint LoadImage(std::string path);
};
