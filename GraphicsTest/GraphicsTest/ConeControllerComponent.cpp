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


#define SOMENUMBER 1
#define ADJUSTANGLE 5
#define SIGHTDISTANCE 20.0f




ConeControllerComponent::ConeControllerComponent() : ControllerControllerComponent(), mDirX(1.0), mDirY(0.0), mAngle(0.0f), mSightRadius(30.0f)
{
  for (int i = 0; i < 10; ++i) leds[i] = 0;
  garbage = 0;
  mName_ = "ConeControllerComponent";
}

bool ConeControllerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mPCPlayer = o->GetFirstItemByName("Player");
  mKnife = o->GetFirstItemByName("Knife");
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

    if (GetKillable())
    {
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

}

void ConeControllerComponent::Shutdown()
{
  mPCPlayer = nullptr;
}

void ConeControllerComponent::UpdateCone()
{
  UpdateVis(mPCPlayer);
  UpdateVis(mKnife);
}

void ConeControllerComponent::UpdateVis(Object* theTarget)
{
  auto * rigid = mParent()->GetComponent(BoxColliderComponent);
  auto * otherRigid = theTarget->GetComponent(BoxColliderComponent);

  b2Vec2 theDistance(rigid->GetBody()->GetPosition().x - otherRigid->GetBody()->GetPosition().x, rigid->GetBody()->GetPosition().y - otherRigid->GetBody()->GetPosition().y);
  b2Vec2 absol;
  absol.x = theDistance.x * theDistance.x;
  absol.y = theDistance.y * theDistance.y;
  if (absol.x + absol.y > SIGHTDISTANCE)
  {
    if (theTarget->mVisible == true)
    {
      theTarget->hasChanged = true;
      theTarget->mVisible = false;
    }
    return;
  }
  theDistance.Normalize();
  float theCosine = mDirX * theDistance.x + mDirY * theDistance.y;
  if (-theCosine > 0.9f)
  {
    if (theTarget->mVisible == false)
    {
      theTarget->hasChanged = true;
      theTarget->mVisible = true;
    }
  }
  else
  {
    if (theTarget->mVisible == true)
    {
      theTarget->hasChanged = true;
      theTarget->mVisible = false;
    }
  }
}

void ConeControllerComponent::SpecialFunctionality(InputSystem* input)
{
  //static int test = 1;
  Controller* coneCon = &input->getController(GetControllerID());
  if (coneCon->knobDelta != 0)
  {
    if (coneCon->knobDelta < 0){
      counterclockwise += coneCon->knobDelta;
    }
    if (coneCon->knobDelta > 0){
      clockwise += coneCon->knobDelta;
    }
  }
  if (clockwise > SOMENUMBER)
  {
    mAngle -= clockwise * ADJUSTANGLE;  
    float rads = radians(mAngle);
    mDirX = cosf(rads);
    mDirY = sinf(rads);
    clockwise = 0;
    counterclockwise = 0;
    
  }
  else if (counterclockwise < -SOMENUMBER){
    mAngle -= counterclockwise * ADJUSTANGLE;  
    float rads = radians(mAngle);
    mDirX = cosf(rads);
    mDirY = sinf(rads);
    clockwise = 0;
    counterclockwise = 0;
  }

  //if (mAngle > 360 || mAngle< 0){
  //  test *= -1;
  //}
  //mAngle -= ADJUSTANGLE * test;

}