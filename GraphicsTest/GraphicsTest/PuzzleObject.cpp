#include "PuzzleObject.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "CircleColliderComponent.h"
#include "PuzzlePi.h"
#include "PuzzleHints.h"

PuzzleObject::PuzzleObject() : GameLogicComponent(GameLogicType::PUZZLEOBJECT), hintValue_(0), clicked_(false), collide_(false)
{
  mName_ = "PuzzleObject";
}

bool PuzzleObject::Initialize()
{
  return true;
}

void PuzzleObject::Update(double dt)
{
  if (!clicked_ && collide_)
  {

    auto* i = gCore->GetSystem(InputSystem);
    //Pushing down on the right joystick
    if (i->isButtonJustPressed(0,1))
    {
      clicked_ = true;
    }
  }
  if (clicked_)
  {
    auto* o = gCore->GetSystem(ObjectSystem);
    if (hintValue_ > 0)
    {
      auto* obj = o->GetFirstItemByName("HintHolder");
      obj->GetComponent(PuzzleHints)->SetHintRevealed(hintValue_);
    }
    else
    {
      auto* obj = o->GetFirstItemByName("Arrow");
      obj->GetComponent(PuzzlePi)->UpdateRange();
    }
  }
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONENDED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "Checker")
        {
          collide_ = false;
        }
      }
      else
      {
        if (col->obj1->name == "Checker")
        {
          collide_ = false;
        }
      }
    }
    else if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "Checker")
        {
          collide_ = true;
        }
      }
      else
      {
        if (col->obj1->name == "Checker")
        {
          collide_ = true;
        }
      }
    }
  }
}

void PuzzleObject::Shutdown()
{

}