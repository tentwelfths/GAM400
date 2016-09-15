#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent() : PhysicsComponent(PhysicsType::BOXCOLLIDER)
{
  //AddMember(BoxColliderComponent, mGhost);
  AddMember(BoxColliderComponent, mHalfSize);
  AddMember(BoxColliderComponent, mOffset);
  
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