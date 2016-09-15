#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "Object.h"
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
  auto trans = mParent_->GetComponent(TransformComponent);
  float x = (trans->mPositionX() + mVelocity.x) * dt;
  float y = (trans->mPositionY() + mVelocity.y) * dt;
  float z = (trans->mPositionZ() + mVelocity.z) * dt;
  vec3 newPos(x, y, z);
  trans->mPosition(newPos);
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