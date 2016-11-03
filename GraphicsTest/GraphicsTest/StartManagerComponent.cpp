#include "StartManagerComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"

StartManagerComponent::StartManagerComponent() : GameLogicComponent(GameLogicType::START), mP1Ready(false), mP2Ready(false), mCountDown(0.0f), mTarget(1.5f)
{
  mName_ = "StartManagerComponent";
}

bool StartManagerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mPCPlayer = o->GetFirstItemByName("Player");
  mConeMan = o->GetFirstItemByName("Coneman");
  return true;
}

void StartManagerComponent::Update(double dt)
{
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "Player" || col->obj2->name == "Coneman")
        {
          SetBool(col->obj2, true);
        }
      }
      else if (col->obj2 == mParent())
      {
        if (col->obj1->name == "Player" || col->obj1->name == "Coneman")
        {
          SetBool(col->obj1, true);
        }
      }
    }
    else if (iter.type == MessageType::COLLISIONENDED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "Player" || col->obj2->name == "Coneman")
        {
          SetBool(col->obj2, false);
        }
      }
      else if (col->obj2 == mParent())
      {
        if (col->obj1->name == "Player" || col->obj1->name == "Coneman")
        {
          SetBool(col->obj1, false);
        }
      }
    }
  }
  if (mP1Ready && mP2Ready)
  {
    mCountDown += dt;
  }
  else
  {
    mCountDown = 0.0f;
  }
  if (mCountDown > mTarget)
  {
    gCore->LoadLevel("DemoLevel.json");
  }
}

void StartManagerComponent::Shutdown()
{

}

void StartManagerComponent::SetBool(Object* thePlayer, bool state)
{
  if (thePlayer->name == "Player")
  {
    mP1Ready = state;
  }
  else if (thePlayer->name == "Coneman")
  {
    mP1Ready = state;
  }
}