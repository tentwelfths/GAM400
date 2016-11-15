#include "DamageArrowComponent.h"
#include "JSONTranslator.h"
#include "Object.h"
#include "MessagingSystem.h"
#include "BulletComponent.h"
#include "KnifeComponent.h"
#include "TransformComponent.h"
#include "PCControllerComponent.h"
#include "DamageLocatorComponent.h"

DamageArrowComponent::DamageArrowComponent() : GameLogicComponent(GameLogicType::DAMAGEARROW)
{
  mName_ = "DamageArrowComponent";
}
bool DamageArrowComponent::Initialize()
{
  for (int i = 0; i < 5; ++i)
    mParent()->mVisibility[i] = false;
  return true;
}
void DamageArrowComponent::Update(double dt)
{
  if (mVis){
    mTimeAlive += dt;
    if (mTimeAlive >= 0.75){
      mVis = false;
      mTimeAlive = 0;
      for (int i = 0; i < 5; ++i)
        mParent()->mVisibility[i] = false;
    }
  }
}
void DamageArrowComponent::Shutdown()
{
}
void DamageArrowComponent::Show(bool vis[5])
{
  for (int i = 0; i < 5; ++i){
    mParent()->mVisibility[i] = vis[i];
  }
  mVis = true;
}
