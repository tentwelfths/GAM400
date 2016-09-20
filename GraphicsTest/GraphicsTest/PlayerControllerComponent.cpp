#include "PlayerControllerComponent.h"
#include "InputSystem.h"
#include "Globals.h"
#include "Core.h"
#include "glfw3.h"
#include "Object.h"
#include "RigidbodyComponent.h"

PlayerControllerComponent::PlayerControllerComponent() : GameLogicComponent(GameLogicType::PLAYER), speed(1.1f)
{
  AddMember(PlayerControllerComponent, speed);
  mName_ = "PlayerControllerComponent";
}

bool PlayerControllerComponent::Initialize()
{
  return true;
}

void PlayerControllerComponent::Update(double dt)
{
  auto input = gCore->GetSystem(InputSystem);
  auto rigid = mParent_->GetComponent(RigidbodyComponent);
  vec3 newVel;
  if (rigid)
  {
    if (input->isKeyPressed(GLFW_KEY_W))
    {
      newVel.y = speed;
    }
    else if (input->isKeyPressed(GLFW_KEY_S))
    {
      newVel.y = -speed;
    }
    if (input->isKeyPressed(GLFW_KEY_A))
    {
      newVel.x = -speed;
    }
    else if (input->isKeyPressed(GLFW_KEY_D))
    {
      newVel.x = speed;
    }
    rigid->SetVel(newVel);
  }

}

void PlayerControllerComponent::Shutdown()
{

}