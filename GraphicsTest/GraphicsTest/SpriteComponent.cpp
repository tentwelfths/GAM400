#include "SpriteComponent.h"
#include "Globals.h"
#include "Core.h"
#include "GraphicsSystem.h"
#include "Object.h"
#include "MessagingSystem.h"
#include "SOIL.h"

SpriteComponent::SpriteComponent() : GraphicsComponent(GraphicsType::SPRITE)
{
  auto * g = gCore->GetSystem(GraphicsSystem);

  Option comboBox(Options::SCROLLBOX);
  Option floatMinMax(Options::FLOATRANGE);

  comboBox.list.reserve(g->mTextureMap_.size());

  for (auto kv : g->mTextureMap_) {
    comboBox.list.push_back(kv.first);
  }
  floatMinMax.fmin = 0.f;
  floatMinMax.fmax = 1.f;
  AddSpecialMember(SpriteComponent, mTextureName, comboBox);

  AddSpecialMember(SpriteComponent, mTint_, floatMinMax);
  AddSpecialMember(SpriteComponent, mOpacity_, floatMinMax);
  hasChanged = false;
  mName_ = "SpriteComponent";
  mTint_.x = 0.f;
  mTint_.y = 0.f;
  mTint_.z = 0.f;
  mOpacity_ = 1.f;
}

bool SpriteComponent::Initialize()
{
  hasChanged = false;
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
  if (hasChanged){
    IMessage msg(MessageType::SPRITECHANGED);
    SpriteChangeMessage* msgData = reinterpret_cast<SpriteChangeMessage*>(msg.data);
    
    msgData->objID = mParent()->ID;

    MessagingSystem* m = gCore->GetSystem(MessagingSystem);
    m->SendMessageToSystem(msg, "NetworkingSystem");
    hasChanged = false;
  }
}

void SpriteComponent::Shutdown()
{

}

void SpriteComponent::SetTexture(std::string textureName)
{
  hasChanged = true;
  mTextureName = textureName;
  mTextureIndex = gCore->GetSystem(GraphicsSystem)->mTextureMap_.find(textureName)->second.index;
  //if (temp > 0)
  //  mTexture_ = temp;

}

void      SpriteComponent::mTint(float r, float g, float b)
{
  hasChanged = true;
  mTint_.r = r;
  mTint_.g = g;
  mTint_.b = b;
}
void      SpriteComponent::mTint(glm::vec3 tint)
{
  hasChanged = true;
  mTint_ = tint;
}
void      SpriteComponent::mOpacity(float a)
{
  hasChanged = true;
  mOpacity_ = a;
}
glm::vec3 SpriteComponent::mTint()
{
  return mTint_;
}
float     SpriteComponent::mOpacity()
{
  return mOpacity_;
}
