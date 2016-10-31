#include <Box2D\Box2D.h>
#include "ControllerControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "JSONTranslator.h"
#include "MessagingSystem.h"
#include "Messages.h"

ControllerControllerComponent::ControllerControllerComponent() : PlayerControllerComponent(), maxAmmo(10), bulletSpeed(0.0f), shotTimer(1.0f), shotCD(1.0f), controllerID(0)
{
  AddMember(ControllerControllerComponent, bulletSpeed);
  AddMember(ControllerControllerComponent, controllerID);
  mName_ = "ControllerControllerComponent";
}

bool ControllerControllerComponent::Initialize()
{
  IMessage msg(MessageType::CHANGELEDS);
  ChangeLEDSMessage* msgData = reinterpret_cast<ChangeLEDSMessage*>(msg.data);
  msgData->controllerNum = controllerID;
  for (int i = 0; i < maxAmmo; ++i)
  {
    msgData->state[i] = true;
  }

  MessagingSystem* m = gCore->GetSystem(MessagingSystem);
  m->SendMessageToSystem(msg, "NetworkingSystem");
  currAmmo = maxAmmo;
  return true;
}

void ControllerControllerComponent::Update(double dt)
{

}

void ControllerControllerComponent::Shutdown()
{

}

void ControllerControllerComponent::Movement(InputSystem* input)
{
  auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 newVel(0.0f, 0.0f);
  if (rigid)
  {
    Joystick dir = input->getJoystick(controllerID);
    if (dir.x1Stick > 0.1 || dir.x1Stick < -0.1){
      newVel.x = dir.x1Stick * GetSpeed();
    }
    if (dir.y1Stick > 0.1 || dir.y1Stick < -0.1){
      newVel.y = dir.y1Stick * GetSpeed();
    }
    if (rigid->GetBody())
    {
      rigid->GetBody()->SetLinearVelocity(newVel);
    }
  }
}

void ControllerControllerComponent::Shoot(InputSystem* input, double dt)
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
        JSONTranslator j;
        Object * b;
        b = j.CreateObjectFromFile("Bullet.json");
        b->Initialize();
        auto bTrans = b->GetComponent(TransformComponent);
        auto bBox = b->GetComponent(BoxColliderComponent);
        auto trans = mParent()->GetComponent(TransformComponent);
        bTrans->mPosition(trans->mPosition());
        b2Vec2 boxPos(bTrans->mPositionX(), bTrans->mPositionY());
        bBox->GetBody()->SetTransform(boxPos, trans->mRotationZ());
        bBox->GetBody()->SetLinearVelocity(bulletVel);
        shotTimer = 0.0f;
        currAmmo -= 1;
        
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
  }
  shotTimer += dt;
}

void ControllerControllerComponent::SpecialFunctionality(InputSystem* input)
{

}

void ControllerControllerComponent::Reload(InputSystem* input)
{
  if(input->isButtonJustPressed(controllerID, 0))
  {
    IMessage msg(MessageType::CHANGELEDS);
    ChangeLEDSMessage* msgData = reinterpret_cast<ChangeLEDSMessage*>(msg.data);
    msgData->controllerNum = controllerID;
    for (int i = 0; i < maxAmmo; ++i)
    {
      msgData->state[i] = true;
    }

    MessagingSystem* m = gCore->GetSystem(MessagingSystem);
    m->SendMessageToSystem(msg, "NetworkingSystem");
    currAmmo = maxAmmo;
  }
}