#include <Box2D\Box2D.h>
#include "ControllerControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

ControllerControllerComponent::ControllerControllerComponent() : PlayerControllerComponent()
{
  AddMember(ControllerControllerComponent, controllerID);
  mName_ = "ControllerControllerComponent";
}

bool ControllerControllerComponent::Initialize()
{
  return true;
}

void ControllerControllerComponent::Update(double dt)
{

}

void ControllerControllerComponent::Shutdown()
{

}

void ControllerControllerComponent::Movement(InputSystem* input)
{
  auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 newVel(0.0f, 0.0f);
  if (rigid)
  {
    Joystick dir = input->getJoystick(controllerID);
    newVel.x = dir.x1Stick * GetSpeed();
    newVel.y = dir.y1Stick * GetSpeed();
    if (rigid->GetBody())
    {
      rigid->GetBody()->SetLinearVelocity(newVel);
    }
  }
}

void ControllerControllerComponent::SpecialFunctionality(InputSystem* input)
{

}