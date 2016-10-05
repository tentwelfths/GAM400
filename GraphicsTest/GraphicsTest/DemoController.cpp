#include <Box2D\Box2D.h>
#include "DemoController.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "System.h"
#include "IComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"

DemoController::DemoController() :GameLogicComponent(GameLogicType::DEMOPLAYER), speed(1.1f)
{
  AddMember(DemoController, speed);
  mName_ = "DemoController";
}

bool DemoController::Initialize()
{
  return true;
}

void DemoController::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 newVel(0.0f, 0.0f);
  if (rigid)
  {
    Joystick j = input->getJoystick(0);
    if (j.xStick > 0.1 || j.xStick < -0.1)
    {
      newVel.x = j.xStick * speed;
    }
    if (rigid->GetBody())
    {
      rigid->GetBody()->SetLinearVelocity(newVel);
    }
  }
  if (rigid->GetJustCollided())
  {
    mParent()->Destroy();
  }
  auto * trans = mParent_->GetComponent(TransformComponent);
  if (trans->mPositionX() > 10.0f)
  {
    trans->mPositionX(-9.9f);
    rigid->GetBody()->SetTransform(b2Vec2(trans->mPositionX(), trans->mPositionY()),trans->mRotationZ());
  }
  else if (trans->mPositionX() < -10.0f)
  {
    trans->mPositionX(9.9f);
    rigid->GetBody()->SetTransform(b2Vec2(trans->mPositionX(), trans->mPositionY()), trans->mRotationZ());
  }
}

void DemoController::Shutdown()
{

}