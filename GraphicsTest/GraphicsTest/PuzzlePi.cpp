#include <Box2D\Box2D.h>
#include "PuzzlePi.h"
#include "InputSystem.h"
#include "MessagingSystem.h"
#include "ObjectSystem.h"
#include "CircleColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

PuzzlePi::PuzzlePi() : GameLogicComponent(GameLogicType::PUZZLEPI), targetValueX_(0), targetValueY_(0), range_(0), speed_(1), delay_(1.0f), timeTillChange_(0.0f), boomTime_(3.0f), nextUnlock_(0), countDown_(true), start_(false)
{
  AddMember(PuzzlePi, range_);
  AddMember(PuzzlePi, speed_);
  AddMember(PuzzlePi, delay_);
  AddMember(PuzzlePi, nextUnlock_);
  mName_ = "PuzzlePi";
}

bool PuzzlePi::Initialize()
{
  return true;
}

void PuzzlePi::Update(double dt)
{
  if (start_)
  {
    auto* circle = mParent()->GetComponent(CircleColliderComponent);
    UpdateTarget(circle, dt);
  }
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "PlayerLeft")
        {
          start_ = true;
          countDown_ = false;
        }
      }
      else
      {
        if (col->obj1->name == "PlayerLeft")
        {
          start_ = true;
          countDown_ = false;
        }
      }
      if (iter.type == MessageType::COLLISIONENDED)
      {
        CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
        if (col->obj1 == mParent())
        {
          if (col->obj2->name == "PlayerLeft")
          {
            countDown_ = true;
          }
        }
        else
        {
          if (col->obj1->name == "PlayerLeft")
          {
            countDown_ = true;
          }
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

void PuzzlePi::UpdateTarget(CircleColliderComponent* circle, double dt)
{
  if (countDown_)
  {
    boomTime_ -= dt;
  }
  else if (boomTime_ <= 3.0f)
  {
    boomTime_ += dt;
  }
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
    b2Vec2 move(targetValueX_ * speed_, targetValueY_ * speed_);
    circle->GetBody()->SetLinearVelocity(move);
  }
  else
  {
    timeTillChange_ += dt;
  }
}