#include "GoHomeComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "InputSystem.h"

GoHomeComponent::GoHomeComponent() : GameLogicComponent(GameLogicType::HOME)
{
  mName_ = "GoHomeComponent";
}

bool GoHomeComponent::Initialize()
{
  return true;
}

void GoHomeComponent::Update(double dt)
{
  auto * i = gCore->GetSystem(InputSystem);
  if (i->isKeyPressed(GLFW_KEY_SPACE))
  {
    gCore->UnloadLevel();
    gCore->LoadLevel("nothing.json");
  }
}

void GoHomeComponent::Shutdown()
{

}