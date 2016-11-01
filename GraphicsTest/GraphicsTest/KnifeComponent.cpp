#include <Box2D\Box2D.h>
#include "KnifeComponent.h"
#include "Core.h"
#include "Globals.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"

#define EPIFORTRANS 0.00001

KnifeComponent::KnifeComponent() : GameLogicComponent(GameLogicType::KNIFE), offset(0.0, 0.0, 0.0), attackTimer(0.5f), timeTillNextAttack(0.5f), dist(20.0f)
{
  auto * o = gCore->GetSystem(ObjectSystem);
  thePlayer = o->GetFirstItemByName("Player");
  mName_ = "KnifeComponent";
}

bool KnifeComponent::Initialize()
{
  mParent()->mVisible = false;
  return true;
}

void KnifeComponent::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  auto* trans = mParent()->GetComponent(TransformComponent);
  auto* box = mParent()->GetComponent(BoxColliderComponent);
  auto* playerTrans = thePlayer->GetComponent(TransformComponent);
  b2Vec2 check;
  check.x = 0;
  check.y = 0;
  if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_LEFT))
  {
    if (timeTillNextAttack >= attackTimer)
    {
      float x = input->GetMouseX() - trans->mPositionX();
      float y = input->GetMouseY() - trans->mPositionY();
      check.x = x, check.y = y;
      check.Normalize();
      check.x *= dist;
      check.y *= dist;
      //offset.x = check.x;
      //offset.y = check.y;
      //offset *= dist;
      box->GetBody()->SetLinearVelocity(check);
      timeTillNextAttack = 0.0f;
    }
  }
  bool xCheck = trans->mPositionX() > playerTrans->mPositionX() - EPIFORTRANS && trans->mPositionX() < playerTrans->mPositionX() + EPIFORTRANS;
  bool yCheck = trans->mPositionX() > playerTrans->mPositionY() - EPIFORTRANS && trans->mPositionX() < playerTrans->mPositionY() + EPIFORTRANS;
  if (timeTillNextAttack > 0.1f && (!xCheck || !yCheck))
  {
    trans->mPosition(playerTrans->mPosition());
    b2Vec2 physicsPos(playerTrans->mPositionX(), playerTrans->mPositionY());
    box->GetBody()->SetTransform(physicsPos, playerTrans->mRotationZ());
  }
  timeTillNextAttack += dt;
}

void KnifeComponent::Shutdown()
{

}