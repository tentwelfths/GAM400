#include <Box2D\Box2D.h>
#include "PuzzlePi.h"
#include "InputSystem.h"
#include "MessagingSystem.h"
#include "ObjectSystem.h"
#include "CircleColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

PuzzlePi::PuzzlePi() : GameLogicComponent(GameLogicType::PUZZLEPI), targetValueX_(0), targetValueY_(0), range_(0.7f), speed_(1), delay_(1.0f), countdown_(0.0f), killtime_(1.5f), timeTillChange_(0.0f), dying_(false)
{
  AddMember(PuzzlePi, range_);
  AddMember(PuzzlePi, speed_);
  AddMember(PuzzlePi, delay_);
  mName_ = "PuzzlePi";
}

bool PuzzlePi::Initialize()
{
  return true;
}

void PuzzlePi::Update(double dt)
{
  UpdateTarget(dt);
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONENDED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "Sphere")
        {
          dying_ = true;
        }
      }
      else
      {
        if (col->obj1->name == "Sphere")
        {
          dying_ = true;
        }
      }
    }
    else if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "Sphere")
        {
          dying_ = false;
        }
      }
      else
      {
        if (col->obj1->name == "Sphere")
        {
          dying_ = false;
        }
      }
    }
    //if (iter.type == MessageType::COLLISIONENDED)
    //{
    //  CollisionEndedMessage * col = reinterpret_cast<CollisionEndedMessage *>(iter.data);
    //}
  }
}

void PuzzlePi::Shutdown()
{

}

void PuzzlePi::UpdateTarget( double dt)
{
  auto * circle = mParent()->GetComponent(CircleColliderComponent);
  auto * i = gCore->GetSystem(InputSystem);
  b2Vec2 move(((targetValueX_ * range_) + i->getJoystick(0).x1Stick) * speed_, ((targetValueY_ * range_) + i->getJoystick(0).y1Stick) * speed_);
  if (delay_ <= timeTillChange_)
  {
    float randX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float randY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (rand() % 2 == 1)
    {
      randX = -randX;
    }
    if (rand() % 2 == 1)
    {
      randY = -randY;
    }
    
    targetValueX_ = randX;
    targetValueY_ = randY;
    timeTillChange_ = 0.0f;
    
  }
  else
  {
    timeTillChange_ += dt;
  }
  circle->GetBody()->SetLinearVelocity(move);
  if (dying_)
  {
    auto sprite = mParent()->GetComponent(SpriteComponent);
    sprite->mTint_.b -= dt;
    sprite->mTint_.g -= dt;
    countdown_ += dt;
  }
  if (countdown_ > killtime_)
  {

  }
}

void PuzzlePi::UpdateRange()
{
  range_ += 0.01f;
}