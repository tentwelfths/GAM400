#include "CloudComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

CloudComponent::CloudComponent() : GameLogicComponent(GameLogicType::CLOUD), lifeTime(0.5f)
{
  mName_ = "CloudComponent";
}

bool CloudComponent::Initialize()
{
  return true;
}

void CloudComponent::Update(double dt)
{
  if (lifeTime < 0)
  {
    mParent()->Destroy();
  }
  lifeTime -= dt;
}

void CloudComponent::Shutdown()
{

}