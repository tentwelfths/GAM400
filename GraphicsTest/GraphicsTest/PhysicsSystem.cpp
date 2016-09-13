#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

PhysicsSystem::PhysicsSystem()
{
  mName_ = "PhysicsSystem";
}

bool PhysicsSystem::Initialize()
{
  return true;
}

void PhysicsSystem::Update(double dt)
{
  for (auto & iter : mComponents_)
  {
    iter->Update(dt);
  }
}

void PhysicsSystem::Shutdown()
{

}

void PhysicsSystem::RegisterComponent(PhysicsComponent * comp)
{
  mComponents_.push_back(comp);
}