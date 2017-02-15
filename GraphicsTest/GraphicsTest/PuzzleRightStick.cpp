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

PuzzleRightStick::PuzzleRightStick() : GameLogicComponent(GameLogicType::ARROW), timeLeft_(1.0f), timeReset_(1.0f), completed_(0)
{
  mName_ = "PuzzleRightStick";
}

bool PuzzleRightStick::Initialize()
{
  srand(time(0));
  RandomArrow();
  return true;
}
void PuzzleRightStick::Update(double dt)
{
  auto* i = gCore->GetSystem(InputSystem);
  Joystick* j = &i->getJoystick(0);
  timeLeft_ -= dt;
  if (theArrow_ == PuzzleArrow::DOWN && j->y2Stick < -.025)
  {
    ++completed_;
    timeLeft_ = timeReset_;
    RandomArrow();
  }
  else if (theArrow_ == PuzzleArrow::UP && j->y2Stick > .025)
  {
    ++completed_;
    timeLeft_ = timeReset_;
    RandomArrow();
  }
  else if (theArrow_ == PuzzleArrow::RIGHT && j->y2Stick > .025)
  {
    ++completed_;
    timeLeft_ = timeReset_;
    RandomArrow();
  }
  else if (theArrow_ == PuzzleArrow::LEFT && j->y2Stick < -.025)
  {
    ++completed_;
    timeLeft_ = timeReset_;
    RandomArrow();
  }
}

void PuzzleRightStick::Shutdown()
{

}

void PuzzleRightStick::RandomArrow()
{
  auto* trans = mParent()->GetComponent(TransformComponent);
  auto temp = theArrow_;
  while (temp == theArrow_)
  {
    theArrow_ = PuzzleArrow(rand() % 4);
  }
  if (theArrow_ == PuzzleArrow::DOWN)
  {
    trans->mRotationZ(180.0f);
  }
  else if (theArrow_ == PuzzleArrow::UP)
  {
    trans->mRotationZ(0.0f);
  }
  else if (theArrow_ == PuzzleArrow::RIGHT)
  {
    trans->mRotationZ(270.0f);
  }
  else if (theArrow_ == PuzzleArrow::LEFT)
  {
    trans->mRotationZ(90.0f);
  }
}