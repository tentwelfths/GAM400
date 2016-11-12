#include <Box2D\Box2D.h>
#include "PCControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "MessagingSystem.h"
#include "ObjectSystem.h"
#include "BulletComponent.h"
#include "JSONTranslator.h"

PCControllerComponent::PCControllerComponent() : PlayerControllerComponent(), curCam(0), sprintSpeed(2.0f), sprintTime(1.0f), spawnTime(0.125f), isSprinting(false)
{
  AddMember(PCControllerComponent, sprintSpeed);
  mName_ = "PCControllerComponent";
}

bool PCControllerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mHealthBar = o->GetFirstItemByName("PCHealthBar");
  for (int i = 0; i < 4; ++i)
    mParent()->mVisibility[i] = false;
  return true;
}

void PCControllerComponent::Update(double dt)
{
  if (GetAlive())
  {
    auto * input = gCore->GetSystem(InputSystem);
    Movement(input);
    sprintTime += dt;
    if (GetKillable())
    {
      for (auto iter : mParent()->mMessages_)
      {
        if (iter.type == MessageType::COLLISIONSTARTED)
        {
          CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
          if (col->obj1 == mParent())
          {
            if (col->obj2->name == "Bullet")
            {
              Damage(col->obj2->GetComponent(BulletComponent)->GetDamage());
            }
          }
          else
          {
            if (col->obj1->name == "Bullet")
            {
              Damage(col->obj1->GetComponent(BulletComponent)->GetDamage());
            }
          }
        }
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
    if (mHealthBar){
      if (mParent()->hasChanged)
      {
        if (mHealthBar->mVisibility[0] != mParent()->mVisibility[0]) mHealthBar->hasChanged = true;
        mHealthBar->mVisibility[0] = mParent()->mVisibility[0];
      }
      auto * trans = mHealthBar->GetComponent(TransformComponent);
      auto * transPlayer = mParent()->GetComponent(TransformComponent);
      float x = transPlayer->mPositionX();
      float y = transPlayer->mPositionY() - 0.9f;
      bool xCheck = trans->mPositionX() > x - EPIFORTRANS && trans->mPositionX() < x + EPIFORTRANS;
      bool yCheck = trans->mPositionY() > y - EPIFORTRANS && trans->mPositionY() < y + EPIFORTRANS;

      if (!xCheck || !yCheck)
      {
        trans->mPosition(x, y, 0);
      }
    }
  }
}

void PCControllerComponent::Shutdown()
{

}

void PCControllerComponent::Damage(int damage) 
{ 
  health -= damage; 
  if (mHealthBar){
    mHealthBar->GetComponent(TransformComponent)->mScaleX(2.f * (health / 10.f));
    mHealthBar->hasChanged = true;
  }
}

void PCControllerComponent::Movement(InputSystem* input)
{
  auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 newVel(0.0f,0.0f);
  if (rigid)
  {

    if (input->isKeyPressed(GLFW_KEY_W))
    {
      newVel.y = GetSpeed();
    }
    else if (input->isKeyPressed(GLFW_KEY_S))
    {
      newVel.y = -GetSpeed();
    }
    if (input->isKeyPressed(GLFW_KEY_A))
    {
      newVel.x = -GetSpeed();
    }
    else if (input->isKeyPressed(GLFW_KEY_D))
    {
      newVel.x = GetSpeed();
    }
    if (input->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
      if (sprintTime > spawnTime)
      {
        JSONTranslator j;
        Object * b;
        b = j.CreateObjectFromFile("CloudSprint.json");
        b->Register();
        b->Initialize();
        auto* trans = mParent()->GetComponent(TransformComponent);
        auto* bTrans = b->GetComponent(TransformComponent);
        bTrans->mPosition(trans->mPosition());
        float x = newVel.x;
        float y = newVel.y;
        float rad = atan2f(y, x);
        rad = degrees(rad);
        bTrans->mRotationZ(rad);
        sprintTime = 0.0f;
      }
      isSprinting = true;
    }
    else
    {
      isSprinting = false;
    }
    if (rigid->GetBody())
    {
      if (isSprinting)
      {
        newVel *= sprintSpeed;
      }
      rigid->GetBody()->SetLinearVelocity(newVel);
    }
  }
}