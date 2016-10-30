#include "Box2D\Box2D.h"
#include "ConeControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "PCControllerComponent.h"
#include "SpriteComponent.h"

ConeControllerComponent::ConeControllerComponent() : ControllerControllerComponent(), mDirX(0.0), mDirY(0.0), mAngle(0.0f), mSightRadius(30.0f)
{
  mName_ = "ConeControllerComponent";
}

bool ConeControllerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mPCPlayer = o->GetFirstItemByName("Player");
  return true;
}

void ConeControllerComponent::Update(double dt)
{
  if (GetAlive())
  {
    auto * input = gCore->GetSystem(InputSystem);
    Movement(input);
    Shoot(input, dt);
    SpecialFunctionality(input);
    UpdateCone();

    for (auto iter : mParent()->mMessages_)
    {
      if (iter.type == MessageType::COLLISIONSTARTED)
      {
        CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
        if (col->obj1 == mParent())
        {
          if (col->obj2->name == "Knife")
          {
            Damage(1);
          }
        }
        else
        {
          if (col->obj1->name == "Knife")
          {
            Damage(1);
          }
        }
      }
      //if (iter.type == MessageType::COLLISIONENDED)
      //{
      //  CollisionEndedMessage * col = reinterpret_cast<CollisionEndedMessage *>(iter.data);
      //}
    }
    if (GetHealth() <= 0)
    {
      Kill();
      auto * sprite = mParent()->GetComponent(SpriteComponent);
      sprite->SetTexture("bolt.png");
      auto * box = mParent()->GetComponent(BoxColliderComponent);
      box->GetBody()->GetFixtureList()->SetSensor(true);
    }
  }
  
}

void ConeControllerComponent::Shutdown()
{
  mPCPlayer = nullptr;
}

void ConeControllerComponent::UpdateCone()
{
  auto * rigid = mParent()->GetComponent(BoxColliderComponent);
  auto * otherRigid = mPCPlayer->GetComponent(BoxColliderComponent);

  b2Vec2 theDistance(rigid->GetBody()->GetPosition().x - otherRigid->GetBody()->GetPosition().x, rigid->GetBody()->GetPosition().y - otherRigid->GetBody()->GetPosition().y);
  float theCosine = mDirX * theDistance.x + mDirY * theDistance.y;
  if (theCosine < mSightRadius)
  {
    mPCPlayer->mVisable = true;
  }
  else
  {
    mPCPlayer->mVisable = false;
  }

}

void ConeControllerComponent::SpecialFunctionality(InputSystem* input)
{

}