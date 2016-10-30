#include <Box2D\Box2D.h>
#include "PCControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

PCControllerComponent::PCControllerComponent() : PlayerControllerComponent(), curCam(0), sprintSpeed(2.0f), isSprinting(false)
{
  AddMember(PCControllerComponent, sprintSpeed);
  mName_ = "PCControllerComponent";
}

bool PCControllerComponent::Initialize()
{
  mParent()->mVisable = false;
  return true;
}

void PCControllerComponent::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  Movement(input);
}

void PCControllerComponent::Shutdown()
{

}

void PCControllerComponent::Movement(InputSystem* input)
{
  auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 newVel(0.0f,0.0f);
  if (rigid)
  {
    if (input->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
      isSprinting = true;
    }
    else
    {
      isSprinting = false;
    }
    if (input->isKeyPressed(GLFW_KEY_W))
    {
      newVel.y = GetSpeed();
    }
    else if (input->isKeyPressed(GLFW_KEY_S))
    {
      newVel.y = -GetSpeed();
    }
    if (input->isKeyPressed(GLFW_KEY_A))
    {
      newVel.x = -GetSpeed();
    }
    else if (input->isKeyPressed(GLFW_KEY_D))
    {
      newVel.x = GetSpeed();
    }
    if (rigid->GetBody())
    {
      if (isSprinting)
      {
        newVel *= sprintSpeed;
      }
      rigid->GetBody()->SetLinearVelocity(newVel);
    }
  }
}