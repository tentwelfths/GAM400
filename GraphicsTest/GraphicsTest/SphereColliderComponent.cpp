#include "SphereColliderComponent.h"
#include "PhysicsSystem.h"
#include "Core.h"
#include "Globals.h"

SphereColliderComponent::SphereColliderComponent() : PhysicsComponent(PhysicsType::SPHERECOLLIDER)
{
  //AddMember(BoxColliderComponent, mGhost);
  AddMember(SphereColliderComponent, mRadius);
  AddMember(SphereColliderComponent, mOffset);
  PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  g->RegisterComponent(this);
  mName_ = "SphereColliderComponent";
}

bool SphereColliderComponent::Initialize()
{
  return true;
}

void SphereColliderComponent::Update(double dt)
{

}

void SphereColliderComponent::Shutdown()
{

}

void SphereColliderComponent::SetGhost(bool newGhost)
{
  mGhost = newGhost;
}

bool SphereColliderComponent::GetGhost()
{
  return mGhost;
}

void SphereColliderComponent::SetRadius(float newRadius)
{
  mRadius = newRadius;
}

void SphereColliderComponent::SetOffset(vec3 newOffset)
{
  mOffset = newOffset;
}

float SphereColliderComponent::GetRadius()
{
  return mRadius;
}

vec3 SphereColliderComponent::GetOffset()
{
  return mOffset;
}