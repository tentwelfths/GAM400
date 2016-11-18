#include "ChargeObjective.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "LevelManagerComponent.h"
#include "SpriteComponent.h"


ChargeObjective::ChargeObjective() : GameLogicComponent(GameLogicType::CHARGEOBJECTIVE), done(false), charge(0.0f), chargeCap(1.0f)
{
  AddMember(ChargeObjective, chargeCap);
  for (auto iter : playersOn)
  {
    iter = false;
  }
  mName_ = "ChargeObjective";
}

bool ChargeObjective::Initialize()
{
  return true;
}

void ChargeObjective::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  for (int i = 0; i < 4; ++i)
  {
    if (playersOn[i] && input->isButtonPressed(i, 0))
    {
      if (charge < chargeCap)
      {
        charge += dt;
        auto* color = mParent()->GetComponent(SpriteComponent);
        color->mTint_.g -= dt;
        color->mTint_.r -= dt;
      }
    }
  }
  if (!done && charge > chargeCap)
  {
    done = true;
    auto* o = gCore->GetSystem(ObjectSystem);
    Object* manager = o->GetFirstItemByName("LevelManager");
    auto* levelManager = manager->GetComponent(LevelManagerComponent);
    for (int i = 0; i < 2; ++i)
    {
      if (levelManager->mObjectives[i] != true)
      {
        levelManager->mObjectives[i] = true;
        break;
      }
    }
  }
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2 != nullptr)
        {
          if (col->obj2->name == "Coneman")
          {
            playersOn[0] = true;
          }
          else if (col->obj2->name == "Gunman")
          {
            playersOn[1] = true;
          }
        }
      }
      else
      {
        if (col->obj1 != nullptr)
        {
          if (col->obj1 != nullptr)
          {
            if (col->obj1->name == "Coneman")
            {
              playersOn[0] = true;
            }
            else if (col->obj1->name == "Gunman")
            {
              playersOn[1] = true;
            }
          }
        }
      }
    }
    else if (iter.type == MessageType::COLLISIONENDED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 != nullptr && col->obj2 != nullptr)
      {

      }
      else if (col->obj1 != nullptr && col->obj1 == mParent())
      {
        if (col->obj2 != nullptr)
        {
          if ((col->obj2->name == "Coneman"))
          {
            playersOn[0] = false;
          }
          else if (col->obj2->name == "Gunman")
          {
            playersOn[1] = false;
          }
        }
      }
      else if (col->obj2 != nullptr && col->obj2 == mParent())
      {
        if (col->obj1 != nullptr)
        {
          if ((col->obj1->name == "Coneman"))
          {
            playersOn[0] = false;
          }
          else if (col->obj1->name == "Gunman")
          {
            playersOn[1] = false;
          }
        }
      }
    }
  }
}

void ChargeObjective::Shutdown()
{

}