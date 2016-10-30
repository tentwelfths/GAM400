#include <Box2D\Box2D.h>
#include "ControllerControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "JSONTranslator.h"

ControllerControllerComponent::ControllerControllerComponent() : PlayerControllerComponent(), bulletSpeed(0.0f), shotTimer(1.0f), shotCD(1.0f), controllerID(0)
{
  AddMember(ControllerControllerComponent, bulletSpeed);
  AddMember(ControllerControllerComponent, controllerID);
  mName_ = "ControllerControllerComponent";
}

bool ControllerControllerComponent::Initialize()
{
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
      bulletVel.x = joy.x2Stick * bulletSpeed;
    }
    if (joy.y2Stick > 0.1 || joy.y2Stick < -0.1){
      bulletVel.y = joy.y2Stick * bulletSpeed;
    }
    bulletVel.Normalize();
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
    }
  }
  shotTimer += dt;
}

void ControllerControllerComponent::SpecialFunctionality(InputSystem* input)
{

}