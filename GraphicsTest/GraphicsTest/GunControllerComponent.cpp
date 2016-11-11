#include <Box2D\Box2D.h>
#include "GunControllerComponent.h"
#include "InputSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "JSONTranslator.h"
#include "BoxColliderComponent.h"
#include "BulletComponent.h"
#include "MessagingSystem.h"

#define RAPIDTIMER 0.7f
#define PIFOURTH 0.78539816339

GunControllerComponent::GunControllerComponent() : mSpreadOrRapid(false), mBigOrPierce(false), mHomingOrDamage(false)
{
  mName_ = "GunControllerComponent";
}

bool GunControllerComponent::Initialize()
{
  ControllerControllerComponent::Initialize();
  return true;
}

void GunControllerComponent::Update(double dt)
{
  InputSystem* i = gCore->GetSystem(InputSystem);
  Movement(i);
  Shoot(i, dt);
  Reload(i);
  SpecialFunctionality(i);
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
            auto * sprite = mParent()->GetComponent(SpriteComponent);
            sprite->SetTexture("rock.png");
          }
        }
        else
        {
          if (col->obj1->name == "Knife")
          {
            Damage(1);
            auto * sprite = mParent()->GetComponent(SpriteComponent);
            sprite->SetTexture("rock.png");
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

void GunControllerComponent::Shutdown()
{

}

void GunControllerComponent::Shoot(InputSystem* input, double dt)
{
  if (shotTimer >= shotCD)
  {
    b2Vec2 bulletVel(0.0f, 0.0f);
    Joystick joy = input->getJoystick(0);
    if (joy.x2Stick > 0.1 || joy.x2Stick < -0.1){
      bulletVel.x = joy.x2Stick;
    }
    if (joy.y2Stick > 0.1 || joy.y2Stick < -0.1){
      bulletVel.y = joy.y2Stick;
    }
    bulletVel.Normalize();
    bulletVel.x *= bulletSpeed;
    bulletVel.y *= bulletSpeed;

    if (currAmmo > 0)
    {
      if (bulletVel.x > 0.1f || bulletVel.x < -0.1f || bulletVel.y > 0.1f || bulletVel.y < -0.1f)
      {
        if (mSpreadOrRapid)
        {
          float ca = cosf(PIFOURTH);
          float sa = sinf(PIFOURTH);
          CreateBullet(bulletVel.x, bulletVel.y);
          CreateBullet(bulletVel.x * ca - bulletVel.y * sa, bulletVel.y * ca + bulletVel.x * sa);
          CreateBullet(bulletVel.x * ca + bulletVel.y * sa, bulletVel.y * ca - bulletVel.x * sa);
          shotTimer = 0.0f;
        }
        else
        {
          CreateBullet(bulletVel.x, bulletVel.y);
          shotTimer = RAPIDTIMER;
        }
      }
      IMessage msg(MessageType::CHANGELEDS);
      ChangeLEDSMessage* msgData = reinterpret_cast<ChangeLEDSMessage*>(msg.data);
      msgData->controllerNum = controllerID;
      for (int i = 0; i < maxAmmo; ++i)
      {
        if (i < currAmmo)
        {
          msgData->state[i] = true;
        }
        else
        {
          msgData->state[i] = false;
        }
      }

      MessagingSystem* m = gCore->GetSystem(MessagingSystem);
      m->SendMessageToSystem(msg, "NetworkingSystem");
    }
  }
  shotTimer += dt;
}

void GunControllerComponent::SpecialFunctionality(InputSystem* input)
{
  if (input->isButtonPressed(GetControllerID(),1))
  {
    mSpreadOrRapid = true;
  }
  else
  {
    mSpreadOrRapid = false;
  }
  if (input->isButtonPressed(GetControllerID(), 2))
  {
    mBigOrPierce = true;
  }
  else
  {
    mBigOrPierce = false;
  }
  if (input->isButtonPressed(GetControllerID(), 3))
  {
    mHomingOrDamage = true;
  }
  else
  {
    mHomingOrDamage = false;
  }
}

Object* GunControllerComponent::CreateBullet(float x, float y)
{
  JSONTranslator j;
  Object * b;
  b = j.CreateObjectFromFile("Bullet.json");
  b->Register();
  b->Initialize();
  auto bTrans = b->GetComponent(TransformComponent);
  auto bBox = b->GetComponent(BoxColliderComponent);
  auto trans = mParent()->GetComponent(TransformComponent);
  bTrans->mPosition(trans->mPosition());
  b2Vec2 boxPos(bTrans->mPositionX(), bTrans->mPositionY());
  bBox->GetBody()->SetTransform(boxPos, trans->mRotationZ());
  bBox->GetBody()->SetLinearVelocity(b2Vec2(x,y));
  currAmmo -= 1;
  if (mBigOrPierce)
  {
    bTrans->mScale(bTrans->mScaleX() * 2, bTrans->mScaleY() * 2, bTrans->mScaleZ());
  }
  else
  {
    b->GetComponent(BulletComponent)->SetPierce();
  }
  if (mHomingOrDamage)
  {
    b->GetComponent(BulletComponent)->SetHoming();
  }
  else
  {
    b->GetComponent(BulletComponent)->SetDamage(b->GetComponent(BulletComponent)->GetDamage());
  }
  return b;
}