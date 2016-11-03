#include <Box2D\Box2D.h>
#include "PCControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "MessagingSystem.h"

PCControllerComponent::PCControllerComponent() : PlayerControllerComponent(), curCam(0), sprintSpeed(2.0f), isSprinting(false)
{
  AddMember(PCControllerComponent, sprintSpeed);
  mName_ = "PCControllerComponent";
}

bool PCControllerComponent::Initialize()
{
  mParent()->mVisible = false;
  return true;
}

void PCControllerComponent::Update(double dt)
{
  if (GetAlive())
  {
    auto * input = gCore->GetSystem(InputSystem);
    Movement(input);
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
              Damage(1);
            }
          }
          else
          {
            if (col->obj1->name == "Bullet")
            {
              Damage(1);
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
  }
}

void PCControllerComponent::Shutdown()
{

}

void PCControllerComponent::Movement(InputSystem* input)
{
  auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 newVel(0.0f,0.0f);
  if (rigid)
  {
    if (input->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
      isSprinting = true;
    }
    else
    {
      isSprinting = false;
    }
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