#include "SpriteComponent.h"
#include "Globals.h"
#include "Core.h"
#include "GraphicsSystem.h"
#include "Object.h"
#include "SOIL.h"

SpriteComponent::SpriteComponent() : GraphicsComponent(GraphicsType::SPRITE)
{
  AddMember(SpriteComponent, mTextureName);
  AddMember(SpriteComponent, mTint);
  AddMember(SpriteComponent, mOpacity);
  hasChanged = false;
  mName_ = "SpriteComponent";
  mTint.x = 0.f;
  mTint.y = 0.f;
  mTint.z = 0.f;
  mOpacity = 1.f;
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
