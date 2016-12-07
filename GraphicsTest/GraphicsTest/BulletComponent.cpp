#include <Box2D\Box2D.h>
#include "BulletComponent.h"
#include "BoxColliderComponent.h"
#include"ChargeObjective.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

BulletComponent::BulletComponent() : GameLogicComponent(GameLogicType::BULLET), contact(false), pierce(false), homing(false), currTime(0.0f), contactDeathTime(0.0f), timeTillDead(0.1f), damage(2), thePlayer(nullptr), homingTime(0.1f), timeToHome(0.0f)
{
  AddMember(BulletComponent, lifeTime);
  mName_ = "BulletComponent";
}

bool BulletComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  thePlayer = o->GetFirstItemByName("Player");
  return true;
}

void BulletComponent::Update(double dt)
{
  currTime += dt;
  if (currTime > lifeTime)
  {
    mParent()->Destroy();
  }
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        auto* co = col->obj2->GetComponent(ChargeObjective);
        if (col->obj2 != nullptr && (col->obj2->name != "Knife" && col->obj2->name != "Coneman" && col->obj2->name != "Gunman" && col->obj2->name != "Cone" && col->obj2->name != "River" && !co))
        {
          contact = true;
        }
      }
      else
      {
        auto* co = col->obj2->GetComponent(ChargeObjective);
        if (col->obj1 != nullptr && (col->obj1->name != "Knife" && col->obj1->name != "Coneman" && col->obj2->name != "Gunman" && col->obj1->name != "Cone" && col->obj2->name != "River" && !co))
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
  if (homing)
  {
    if (timeToHome > homingTime)
    {
      auto* playerTrans = thePlayer->GetComponent(TransformComponent);
      b2Vec2 velMod(playerTrans->mPositionX(), playerTrans->mPositionY());
      velMod.Normalize();
      auto* box = mParent()->GetComponent(BoxColliderComponent);
      b2Vec2 newVel = box->GetBody()->GetLinearVelocity() + velMod;
      box->GetBody()->SetLinearVelocity(newVel);
      timeToHome = 0.0f;
    }
    timeToHome += dt;
  }
  if (contact && !pierce)
  {
    contactDeathTime += dt;
    if (contactDeathTime > timeTillDead)
    {
      mParent()->Destroy();
    }
  }
}
void BulletComponent::Shutdown()
{

}