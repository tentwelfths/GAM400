#include <Box2D\Box2D.h>
#include "KnifeComponent.h"
#include "Core.h"
#include "Globals.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"

KnifeComponent::KnifeComponent() : GameLogicComponent(GameLogicType::KNIFE), offset(0.0, 0.0, 0.0), attackTimer(0.5f), timeTillNextAttack(0.5f), dist(1.5f)
{
  auto * o = gCore->GetSystem(ObjectSystem);
  thePlayer = o->GetFirstItemByName("Player");
  mName_ = "KnifeComponent";
}

bool KnifeComponent::Initialize()
{
  return true;
}

void KnifeComponent::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  auto* trans = mParent()->GetComponent(TransformComponent);
  auto* box = mParent()->GetComponent(BoxColliderComponent);
  auto* playerTrans = thePlayer->GetComponent(TransformComponent);
  if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_LEFT))
  {
    if (timeTillNextAttack >= attackTimer)
    {
      float x = input->GetMouseX() - trans->mPositionX();
      float y = input->GetMouseY() - trans->mPositionY();
      b2Vec2 check(x, y);
      check.Normalize();
      offset.x = check.x;
      offset.y = check.y;
      offset *= dist;
      timeTillNextAttack = 0.0f;
    }
  }
  trans->mPosition(playerTrans->mPosition() + offset);
  b2Vec2 physicsPos(playerTrans->mPositionX() + offset.x, playerTrans->mPositionY() + offset.y);
  box->GetBody()->SetTransform(physicsPos, playerTrans->mRotationZ());
  if (timeTillNextAttack > 0.2f)
  {
    offset = vec3(0.0, 0.0, 0.0);
  }
  timeTillNextAttack += dt;
}

void KnifeComponent::Shutdown()
{

}