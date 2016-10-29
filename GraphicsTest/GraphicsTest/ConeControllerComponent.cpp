#include "Box2D\Box2D.h"
#include "ConeControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"

ConeControllerComponent::ConeControllerComponent() : ControllerControllerComponent(), mDir(b2Vec2(0.0, 0.0)), mAngle(0.0f), mSightRadius(30.0f)
{
  mName_ = "ConeControllerComponent";
}

bool ConeControllerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mPCPlayer = o->GetFirstItemByName("Player");
  return true;
}

void ConeControllerComponent::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  Movement(input);
  SpecialFunctionality(input);
  UpdateCone();
}

void ConeControllerComponent::Shutdown()
{
  mPCPlayer = nullptr;
}

void ConeControllerComponent::UpdateCone()
{
  auto * rigid = mParent()->GetComponent(BoxColliderComponent);
  auto * otherRigid = mPCPlayer->GetComponent(BoxColliderComponent);

  b2Vec2 theDistance(rigid->GetBody()->GetPosition().x - otherRigid->GetBody()->GetPosition().x, rigid->GetBody()->GetPosition().y - otherRigid->GetBody()->GetPosition().y);
  float theCosine = mDir.x * theDistance.x + mDir.y * theDistance.y;
  if (theCosine < mSightRadius)
  {

  }

}

void ConeControllerComponent::SpecialFunctionality(InputSystem* input)
{

}