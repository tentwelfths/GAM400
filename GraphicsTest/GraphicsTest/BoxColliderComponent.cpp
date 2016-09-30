#include "BoxColliderComponent.h"
#include "PhysicsSystem.h"
#include "Core.h"
#include "Globals.h"

BoxColliderComponent::BoxColliderComponent() : PhysicsComponent(PhysicsType::BOXCOLLIDER), quadLoc(0)
{
  //AddMember(BoxColliderComponent, mGhost);
  AddMember(BoxColliderComponent, mHalfSize);
  AddMember(BoxColliderComponent, mOffset);
  PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  g->RegisterComponent(this);
  mName_ = "BoxColliderComponent";
}

bool BoxColliderComponent::Initialize()
{
  return true;
}

void BoxColliderComponent::Update(double dt)
{

}

void BoxColliderComponent::Shutdown()
{

}

void BoxColliderComponent::SetGhost(bool newGhost)
{
  mGhost = newGhost;
}

bool BoxColliderComponent::GetGhost()
{
  return mGhost;
}

void BoxColliderComponent::SetHalfSize(vec3 newSize)
{
  mHalfSize = newSize;
}

void BoxColliderComponent::SetOffset(vec3 newOffset)
{
  mOffset = newOffset;
}

vec3 BoxColliderComponent::GetHalfSize()
{
  return mHalfSize;
}

vec3 BoxColliderComponent::GetOffset()
{
  return mOffset;
}