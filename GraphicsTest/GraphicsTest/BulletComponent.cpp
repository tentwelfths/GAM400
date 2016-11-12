#include "BulletComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

BulletComponent::BulletComponent() : GameLogicComponent(GameLogicType::BULLET), contact(false), pierce(false), homing(false), currTime(0.0f), contactDeathTime(0.0f), timeTillDead(0.1f), damage(1)
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
        if (col->obj2 != nullptr && (col->obj2->name != "Knife" && col->obj2->name != "Coneman" && col->obj2->name != "Cone"))
        {
          contact = true;
        }
      }
      else
      {
        if (col->obj1 != nullptr && (col->obj1->name != "Knife" && col->obj1->name != "Coneman" && col->obj1->name != "Cone"))
        {
          contact = true;
        }
      }
    }
    //if (iter.type == MessageType::COLLISIONENDED)
    //{
    //  CollisionEndedMessage * col = reinterpret_cast<CollisionEndedMessage *>(iter.data);
    //}
  }
  if (contact && !pierce)
  {
    contactDeathTime += dt;
    if (contactDeathTime > timeTillDead)
    {
      mParent()->dead = true;
    }
  }
}
void BulletComponent::Shutdown()
{

}