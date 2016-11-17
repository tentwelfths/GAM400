#pragma once
#include "GraphicsComponent.h"
#include "GL\glew.h"

class SpriteComponent : public GraphicsComponent
{
public:
  SpriteComponent();
  bool Initialize();
  void Update(double dt);
  void SetTexture(std::string textureName);
  void Shutdown();
  std::string mTextureName;
  unsigned char mTextureIndex;
  glm::vec3 mTint;
  float mOpacity;
  //GLuint mTexture_;

  bool hasChanged;
};
