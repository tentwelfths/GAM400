#include <Box2D\Box2D.h>
#include <stdlib.h>
#include <time.h>
#include "InputSystem.h"
#include "MessagingSystem.h"
#include "ObjectSystem.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "PuzzleRightStick.h"

#define XBOUNDS 10.0f
#define YBOUNDS 8.0f

PuzzleRightStick::PuzzleRightStick() : GameLogicComponent(GameLogicType::PUZZLEARROW)
{
  mName_ = "PuzzleRightStick";
}

bool PuzzleRightStick::Initialize()
{
  return true;
}
void PuzzleRightStick::Update(double dt)
{
  auto * i = gCore->GetSystem(InputSystem);
  Joystick* j = &i->getJoystick(0);
  b2Vec2 moveVec(j->x2Stick, j->y2Stick);
  auto * circle = mParent()->GetComponent(CircleColliderComponent);
  auto * theBody = circle->GetBody();
  theBody->SetLinearVelocity(moveVec);
  auto* trans = mParent()->GetComponent(TransformComponent);
  b2Vec2 temp = theBody->GetTransform().p;
  if (trans->mPositionX() > XBOUNDS)
  {
    temp.x = XBOUNDS - 0.1f;
  }
  else if (trans->mPositionX() < XBOUNDS)
  {
    temp.x = -XBOUNDS + 0.1f;
  }
  if (trans->mPositionY() > YBOUNDS)
  {
    temp.y = YBOUNDS - 0.1f;
  }
  else if (trans->mPositionY() < YBOUNDS)
  {
    temp.y = -YBOUNDS + 0.1f;
  }
}

void PuzzleRightStick::Shutdown()
{

}