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

ConeComponent::ConeComponent() : GameLogicComponent(GameLogicType::CONE)
{
  mName_ = "ConeComponent";
}

bool ConeComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  thePlayer = o->GetFirstItemByName("Coneman");
  return true;
}

void ConeComponent::Update(double dt)
{
  auto * trans = mParent()->GetComponent(TransformComponent);
  auto * transPlayer = thePlayer->GetComponent(TransformComponent);
  auto * theCone = thePlayer->GetComponent(ConeControllerComponent);
  float x = transPlayer->mPositionX() + theCone->GetX() * (trans->mScaleX() / 2.f); 
  float y = transPlayer->mPositionY() + theCone->GetY() * (trans->mScaleY() / 2.f);
  bool xCheck = trans->mPositionX() > x - EPIFORTRANS && trans->mPositionX() < x + EPIFORTRANS;
  bool yCheck = trans->mPositionY() > y - EPIFORTRANS && trans->mPositionY() < y + EPIFORTRANS;

  if (!xCheck || !yCheck)
  {
    trans->mPosition(x, y, trans->mPositionZ());
    trans->mRotationZ((thePlayer->GetComponent(ConeControllerComponent)->GetAngle()));
  }
}

void ConeComponent::Shutdown()
{

}