#include "Box2D\Box2D.h"
#include "ConeComponent.h"
#include "System.h"
#include "IComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "BoxColliderComponent.h"
#include "ConeControllerComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"

#define EPIFORTRANS 0.00001

ConeComponent::ConeComponent() : GameLogicComponent(GameLogicType::CONE)
{
  auto * o = gCore->GetSystem(ObjectSystem);
  thePlayer = o->GetFirstItemByName("Coneman");
  mName_ = "Cone";
}

bool ConeComponent::Initialize()
{
  return true;
}

void ConeComponent::Update(double dt)
{
  auto * trans = mParent()->GetComponent(TransformComponent);
  auto * transPlayer = thePlayer->GetComponent(TransformComponent);
  auto * theCone = thePlayer->GetComponent(ConeControllerComponent);
  float x = transPlayer->mPositionX() + theCone->GetX();
  float y = transPlayer->mPositionY() + theCone->GetY();
  bool xCheck = trans->mPositionX() > x - EPIFORTRANS && trans->mPositionX() < x + EPIFORTRANS;
  bool yCheck = trans->mPositionY() > y - EPIFORTRANS && trans->mPositionY() < y + EPIFORTRANS;

  if (!xCheck || !yCheck)
  {
    trans->mPosition(x, y, transPlayer->mPositionZ() + 1);
  }
}

void ConeComponent::Shutdown()
{

}