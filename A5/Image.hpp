
#pragma once

#include <glm/glm.hpp>
#include "cs488-framework/OpenGLImport.hpp"
#include <string>

class Image {
  public:

    Image() {}
    GLuint loadImage(std::string path);
    GLuint loadImagePNG(std::string path);
};