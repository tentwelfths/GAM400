#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "Core.h"
#include "Globals.h"


PhysicsComponent::PhysicsComponent(PhysicsType t)
{
  PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  g->RegisterComponent(this);

  mName_ = "PhysicsComponent";
  mType_ = t;
}

bool PhysicsComponent::Initialize()
{
  return true;
}

void PhysicsComponent::Update(double dt)
{
}

void PhysicsComponent::Shutdown()
{
}

void PhysicsComponent::ConfirmPosition(){

}