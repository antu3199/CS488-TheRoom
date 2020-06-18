// Winter 2019

#include "Material.hpp"
#include "A5.hpp"



Material::~Material()
{
}

void Material::SetTexture(std::string path)
{
  if (path == "null")
  {
    hasTexture = false;
    return;
  }

  this->texture = this->LoadImage(path);
  hasTexture = true;
}

void Material::SetNormalMap(std::string path) {
  if (path == "null")
  {
    hasNormalMap = false;
    return;
  }

  this->normalMap = this->LoadImage(path);
  hasNormalMap = true;
}

GLuint Material::LoadImage(std::string path)
{
  if (path == "null")
  {
    return (GLuint)0;
  }

  std::string realPath = A5::getAssetPath(path.c_str());
  Image image;
  GLuint imageRef = image.loadImage(realPath);
  
  return imageRef;
}