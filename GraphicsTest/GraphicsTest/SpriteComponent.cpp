#include "SpriteComponent.h"
#include "Globals.h"
#include "Core.h"
#include "GraphicsSystem.h"
#include "Object.h"
#include "SOIL.h"

SpriteComponent::SpriteComponent() : GraphicsComponent(GraphicsType::SPRITE)
{
  AddMember(SpriteComponent, mTextureName);
  hasChanged = false;
  mName_ = "SpriteComponent";
}

bool SpriteComponent::Initialize()
{
  hasChanged = true;
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
  mParent_->hasChanged = true;
  mTextureName = textureName;
  //GLuint temp = gCore->GetSystem(GraphicsSystem)->GetTexture(textureName);
  //if (temp > 0)
  //  mTexture_ = temp;

}
