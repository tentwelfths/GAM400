#include "SphereColliderComponent.h"
#include "PhysicsSystem.h"
#include "Core.h"
#include "Globals.h"

SphereColliderComponent::SphereColliderComponent() : PhysicsComponent(PhysicsType::SPHERECOLLIDER)
{
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