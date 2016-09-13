#include "PhysicsMaterialComponent.h"
#include "Globals.h"
#include "Core.h"
#include "SOIL.h"

PhysicsMaterialComponent::PhysicsMaterialComponent() : PhysicsComponent(PhysicsType::PHYSICSMATERIAL)
{
  //AddMember(PhysicsMaterialComponent, mHighPriority);
  AddMember(PhysicsMaterialComponent, mRestitution);
  AddMember(PhysicsMaterialComponent, mFriction);

  mName_ = "PhysicsMaterialComponent";
}

bool PhysicsMaterialComponent::Initialize()
{
  return true;
}

void PhysicsMaterialComponent::Update(double dt)
{

}

void PhysicsMaterialComponent::Shutdown()
{

}

void PhysicsMaterialComponent::SetPriority(bool newPriority)
{
  mHighPriority = newPriority;
}

void PhysicsMaterialComponent::SetRestitution(float newRestitution)
{
  mRestitution = newRestitution;
}


void PhysicsMaterialComponent::SetFriction(float newFriction)
{
  mFriction = newFriction;
}

bool PhysicsMaterialComponent::GetPriority()
{
  return mHighPriority;
}

float PhysicsMaterialComponent::GetRestitution()
{
  return mRestitution;
}

float PhysicsMaterialComponent::GetFriction()
{
  return mFriction;
}