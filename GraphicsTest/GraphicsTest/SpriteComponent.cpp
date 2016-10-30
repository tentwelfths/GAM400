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
  auto * g = gCore->GetSystem(GraphicsSystem);
  if (g->mTextureMap_.find(mTextureName) != g->mTextureMap_.end())
  {
    if (mTextureIndex != g->mTextureMap_.find(mTextureName)->second.index)
    {
      SetTexture(mTextureName);
    }
  }
}

void SpriteComponent::Shutdown()
{

}

void SpriteComponent::SetTexture(std::string textureName)
{
  mParent_->hasChanged = true;
  mTextureName = textureName;
  mTextureIndex = gCore->GetSystem(GraphicsSystem)->mTextureMap_.find(textureName)->second.index;
  //if (temp > 0)
  //  mTexture_ = temp;

}
