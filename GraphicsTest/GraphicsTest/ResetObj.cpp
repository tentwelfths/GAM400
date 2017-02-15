#include <Box2D\Box2D.h>
#include "ResetObj.h"
#include "InputSystem.h"
#include "MessagingSystem.h"
#include "ObjectSystem.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

ResetObj::ResetObj() : GameLogicComponent(GameLogicType::RESET)
{
  mName_ = "ResetObj";
}

bool ResetObj::Initialize()
{
  return true;
}

void ResetObj::Update(double bt)
{
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      if (col->obj1 == mParent())
      {
        if (col->obj2->name == "PlayerLeft" || col->obj2->name == "TempControl")
        {
          auto* circle = col->obj2->GetComponent(CircleColliderComponent);
          auto* trans = col->obj2->GetComponent(TransformComponent);
          Reset(trans, circle);
        }
      }
      else
      {
        if (col->obj1->name == "PlayerLeft" || col->obj1->name == "TempControl")
        {
          auto* circle = col->obj1->GetComponent(CircleColliderComponent);
          auto* trans = col->obj1->GetComponent(TransformComponent);
          Reset(trans, circle);
        }
      }
    }
  }
}

void ResetObj::Shutdown()
{

}

void ResetObj::Reset(TransformComponent* trans, CircleColliderComponent* circle)
{
  trans->mPosition(0.0, 0.0, 0.0);
  b2Vec2 reset(0.0, 0.0);
  circle->GetBody()->SetTransform(reset, 0);
}