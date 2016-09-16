#include "PlayerControllerComponent.h"
#include "InputSystem.h"
#include "Globals.h"
#include "Core.h"
#include "glfw3.h"
#include "Object.h"
#include "RigidbodyComponent.h"

PlayerControllerComponent::PlayerControllerComponent() : GameLogicComponent(GameLogicType::PLAYER), speed(0.1f)
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
  if (rigid)
  {
    if (input->isKeyPressed(GLFW_KEY_W))
    {
      std::cout << "DUBYA PRESSED" << std::endl;
      rigid->AddVel(0.0f, speed, 0.0f);
    }
    else if (input->isKeyPressed(GLFW_KEY_S))
    {
      rigid->AddVel(0.0f, -speed, 0.0f);
    }
    if (input->isKeyPressed(GLFW_KEY_A))
    {
      rigid->AddVel(-speed, 0.0f, 0.0f);
    }
    else if (input->isKeyPressed(GLFW_KEY_D))
    {
      rigid->AddVel(speed, 0.0f, 0.0f);
    }
  }

}

void PlayerControllerComponent::Shutdown()
{

}