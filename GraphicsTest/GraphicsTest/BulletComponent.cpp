#include "BulletComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

BulletComponent::BulletComponent() : GameLogicComponent(GameLogicType::BULLET), currTime(0.0)
{
  AddMember(BulletComponent, lifeTime);
  mName_ = "BulletComponent";
}

bool BulletComponent::Initialize()
{
  return true;
}

void BulletComponent::Update(double dt)
{
  currTime += dt;
  if (currTime > lifeTime)
  {
    mParent()->dead = true;
  }
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name != "Knife" || col->obj2->name != "Coneman")
        {
          mParent()->dead = true;
        }
      }
      else
      {
        if (col->obj1->name != "Knife" || col->obj2->name != "Coneman")
        {
          mParent()->dead = true;
        }
      }
    }
    //if (iter.type == MessageType::COLLISIONENDED)
    //{
    //  CollisionEndedMessage * col = reinterpret_cast<CollisionEndedMessage *>(iter.data);
    //}
  }
}
void BulletComponent::Shutdown()
{

}