#include "GoHomeComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

GoHomeComponent::GoHomeComponent() : GameLogicComponent(GameLogicType::HOME), mCountDown(0.0f), mTarget(3.0f)
{
  mName_ = "GoHomeComponent";
}

bool GoHomeComponent::Initialize()
{
  return true;
}

void GoHomeComponent::Update(double dt)
{
  if (mCountDown > mTarget)
  {
    gCore->UnloadLevel();
    gCore->LoadLevel("New.json");
  }
  mCountDown += dt;
}

void GoHomeComponent::Shutdown()
{

}