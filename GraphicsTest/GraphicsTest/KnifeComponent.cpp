#include <Box2D\Box2D.h>
#include "KnifeComponent.h"
#include "Core.h"
#include "Globals.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "CircleColliderComponent.h"
#include "PCControllerComponent.h"
#include "MessagingSystem.h"

KnifeComponent::KnifeComponent() : GameLogicComponent(GameLogicType::KNIFE), offset(0.0, 0.0, 0.0), attackTimer(0.5f), timeTillNextAttack(0.5f), dist(20.0f)
{
  auto * o = gCore->GetSystem(ObjectSystem);
  thePlayer = o->GetFirstItemByName("Player");
  mName_ = "KnifeComponent";
}

bool KnifeComponent::Initialize()
{
  for (int i = 0; i < 4; ++i)
    mParent()->mVisibility[i] = false;
  return true;
}

void KnifeComponent::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  auto* trans = mParent()->GetComponent(TransformComponent);
  auto* box = mParent()->GetComponent(CircleColliderComponent);
  auto* playerTrans = thePlayer->GetComponent(TransformComponent);
  auto* alive = thePlayer->GetComponent(PCControllerComponent);
  if (alive->GetAlive())
  {
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
        float rad = atan2f(y, x);
        rad = degrees(rad);
        trans->mRotationZ(rad);
        check.Normalize();
        check.x *= dist;
        check.y *= dist;
        //offset.x = check.x;
        //offset.y = check.y;
        //offset *= dist;
        box->GetBody()->SetLinearVelocity(check);
        timeTillNextAttack = 0.0f;
        IMessage col(MessageType::PLAY3DSOUND);
        Play3DSoundMessage * msgData = reinterpret_cast<Play3DSoundMessage *>(col.data);
        strcpy(msgData->name, "Shoot");
        msgData->listener = trans;
        msgData->source = trans;
        MessagingSystem* m = gCore->GetSystem(MessagingSystem);
        m->SendMessageToSystem(col, "NetworkingSystem");
        m->SendMessageToSystem(col, "AudioSystem");
      }
    }
  }
  bool xCheck = trans->mPositionX() > playerTrans->mPositionX() - EPIFORTRANS && trans->mPositionX() < playerTrans->mPositionX() + EPIFORTRANS;
  bool yCheck = trans->mPositionY() > playerTrans->mPositionY() - EPIFORTRANS && trans->mPositionY() < playerTrans->mPositionY() + EPIFORTRANS;
  if (timeTillNextAttack > 0.1f && (!xCheck || !yCheck))
  {
    trans->mPosition(playerTrans->mPosition());
    b2Vec2 physicsPos(playerTrans->mPositionX(), playerTrans->mPositionY());
    box->GetBody()->SetTransform(physicsPos, playerTrans->mRotationZ());
    box->GetBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
  }
  timeTillNextAttack += dt;

}

void KnifeComponent::Shutdown()
{

}