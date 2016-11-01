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
#include "MessagingSystem.h"
#include "Messages.h"

#define ADJUSTANGLE 5

ConeControllerComponent::ConeControllerComponent() : ControllerControllerComponent(), mDirX(0.0), mDirY(-1.0), mAngle(0.0f), mSightRadius(30.0f)
{
  for (int i = 0; i < 10; ++i) leds[i] = 0;
  garbage = 0;
  mName_ = "ConeControllerComponent";
}

bool ConeControllerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mPCPlayer = o->GetFirstItemByName("Player");
  ControllerControllerComponent::Initialize();
  return true;
}

void ConeControllerComponent::Update(double dt)
{
  if (GetAlive())
  {
    auto * input = gCore->GetSystem(InputSystem);
    Movement(input);
    Shoot(input, dt);
    Reload(input);
    SpecialFunctionality(input);
    UpdateCone();
      //IMessage col(MessageType::CHANGELEDS);
      //ChangeLEDSMessage * msgData = reinterpret_cast<ChangeLEDSMessage *>(col.data);
      //MessagingSystem* m = gCore->GetSystem(MessagingSystem);
      //msgData->controllerNum = 0;
      //for (int i = 0; i < 10; ++i) msgData->state[i] = leds[i];
      //m->SendMessageToSystem(col, "NetworkingSystem");


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
      IMessage msg(MessageType::CHANGETEXTURE);
      ChangeTextureMessage* msgData = reinterpret_cast<ChangeTextureMessage*>(msg.data);

      msgData->objID = mParent()->ID;
      MessagingSystem* m = gCore->GetSystem(MessagingSystem);
      m->SendMessageToSystem(msg, "NetworkingSystem");
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
    mPCPlayer->mVisible = true;
  }
  else
  {
    mPCPlayer->mVisible = false;
  }

}

void ConeControllerComponent::SpecialFunctionality(InputSystem* input)
{
  Controller* coneCon = &input->getController(GetControllerID());
  if (coneCon->knobDelta != 0)
  {
    mAngle += coneCon->knobDelta * ADJUSTANGLE;
    float rads = radians(mAngle);
    mDirX = cos(rads);
    mDirY = sin(rads);
  }
}