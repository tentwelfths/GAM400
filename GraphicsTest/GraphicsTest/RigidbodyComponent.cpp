#include "RigidbodyComponent.h"
#include "Globals.h"
#include "Core.h"

RigidbodyComponent::RigidbodyComponent() : PhysicsComponent(PhysicsType::RIGIDBODY)
{
  //AddMember(RigidbodyComponent, mRotationLocked);
  AddMember(RigidbodyComponent, mMass);
  AddMember(RigidbodyComponent, mVelocity);
  //AddMember(RigidbodyComponent, mDynamic);
  mName_ = "RigidbodyComponent";
}

bool RigidbodyComponent::Initialize()
{
  return true;
}

void RigidbodyComponent::Update(double dt)
{

}

void RigidbodyComponent::Shutdown()
{

}

void RigidbodyComponent::SetLocked(bool newLocked)
{
  mRotationLocked = newLocked;
}

void RigidbodyComponent::SetMass(float newMass)
{
  mMass = newMass;
}

void RigidbodyComponent::SetVel(float x, float y, float z)
{
  mVelocity.x = x;
  mVelocity.y = y;
  mVelocity.z = z;
}

void RigidbodyComponent::SetState(DynamicState newState)
{
  mDynamic = newState;
}

void RigidbodyComponent::AddVel(float x, float y, float z)
{
  mVelocity.x += x;
  mVelocity.y += y;
  mVelocity.z += z;
}

bool RigidbodyComponent::GetLocked()
{
  return mRotationLocked;
}

float RigidbodyComponent::GetMass()
{
  return mMass;
}

vec3 RigidbodyComponent::GetVel()
{
  return mVelocity;
}

DynamicState RigidbodyComponent::GetState()
{
  return mDynamic;
}