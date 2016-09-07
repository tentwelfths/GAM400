#include "SpriteComponent.h"
#include "Globals.h"
#include "Core.h"
#include "GraphicsSystem.h"
#include "SOIL.h"

SpriteComponent::SpriteComponent() : GraphicsComponent(GraphicsType::SPRITE)
{
  RegisterComponentType(SpriteComponent);
  AddMember(SpriteComponent, mTexture_);
  mName_ = "SpriteComponent";
}

bool SpriteComponent::Initialize()
{

  return true;
}



void SpriteComponent::Update(double dt)
{

}

void SpriteComponent::Shutdown()
{

}

void SpriteComponent::SetTexture(std::string textureName)
{
  GLuint temp = gCore->GetSystem(GraphicsSystem)->GetTexture(textureName);
  if (temp > 0)
    mTexture_ = temp;

}
