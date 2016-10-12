#pragma once
#include "GraphicsComponent.h"
#include "GL\glew.h"

class SpriteComponent : public GraphicsComponent
{
public:
  SpriteComponent();
  bool Initialize();
  void Update(double dt);
  //void GetTexture(GLuint textureID);
  //void GetTexture(std::string textureName);
  void SetTexture(std::string textureName);
  void Shutdown();
  //GLuint mTexture(){ return mTexture_; }
  std::string mTextureName;
  unsigned char mTextureIndex;
  //GLuint mTexture_;
  bool hasChanged;
};
