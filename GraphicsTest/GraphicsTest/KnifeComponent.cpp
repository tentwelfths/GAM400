#include <Box2D\Box2D.h>
#include "KnifeComponent.h"
#include "Core.h"
#include "Globals.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"

KnifeComponent::KnifeComponent() : GameLogicComponent(GameLogicType::KNIFE), offset(0.0, 0.0, 0.0), attackTimer(0.5f), timeTillNextAttack(0.5f), dist(5.0f)
{
  auto * o = gCore->GetSystem(ObjectSystem);
  thePlayer = o->GetFirstItemByName("Player");
  mName_ = "KnifeComponent";
}

bool KnifeComponent::Initialize()
{
  mParent()->mVisable = false;
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
  if (timeTillNextAttack > 0.2f)
  {
    trans->mPosition(playerTrans->mPosition() + offset);
    b2Vec2 physicsPos(playerTrans->mPositionX() + offset.x, playerTrans->mPositionY() + offset.y);
    box->GetBody()->SetTransform(physicsPos, playerTrans->mRotationZ());
  }
  timeTillNextAttack += dt;
}

void KnifeComponent::Shutdown()
{

}