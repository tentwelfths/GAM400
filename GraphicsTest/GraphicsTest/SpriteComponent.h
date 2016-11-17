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

  void mTint(float r, float g, float b);
  void mTint(glm::vec3 tint);
  void mOpacity(float a);
  glm::vec3 mTint();
  float mOpacity();
  //GLuint mTexture_;

  glm::vec3 mTint_;
  float mOpacity_;

  bool hasChanged;
};
