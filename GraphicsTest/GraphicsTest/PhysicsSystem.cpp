#include "PhysicsSystem.h"
#include "Object.h"
#include "PhysicsComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "TransformComponent.h"
#include "Collision.h"

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
  for (auto & iter : mRigidbodies_)
  {
    iter->Update(dt);
  }
  for (auto & iter : mBoxColliders_)
  {
    iter->Update(dt);
    for (auto iter2 = iter; iter2 != mBoxColliders_.back(); ++iter2)
    {
      auto firstTrans = iter->mParent()->GetComponentA<TransformComponent>("TransformComponent");
      auto secondTrans = iter2->mParent()->GetComponentA<TransformComponent>("TransformComponent");
      auto firstBox = iter->mParent()->GetComponentA<BoxColliderComponent>("BoxColliderComponent");
      auto secondBox = iter2->mParent()->GetComponentA<BoxColliderComponent>("BoxColliderComponent");

      auto firstPos = firstTrans->mPosition() + firstBox->GetOffset();
      auto secondPos = secondTrans->mPosition() + secondBox->GetOffset();
      if ((firstPos.x < (secondPos.x + secondBox->GetHalfSize().x)) &&
        ((firstPos.x + firstBox->GetHalfSize().x) > secondPos.x) &&
        (firstPos.y < (secondPos.y + secondBox->GetHalfSize().y)) && 
        ((firstPos.y + firstBox->GetHalfSize().y) > secondPos.y))
      {
        Collision col;
        col.AddObjects(firstTrans->mParent(), secondTrans->mParent());
        mCollision_.push_back(col);
      }
    }
    for (auto & iter2 : mSphereColliders_)
    {
      
    }
  }
  for (auto & iter : mSphereColliders_)
  {
    iter->Update(dt);
    for (auto iter2 = iter; iter2 != mSphereColliders_.back(); ++iter2)
    {

    }
  }
  for (auto & iter : mCollision_)
  {
    iter.ResolveCollision();
  }
  mCollision_.clear();
}

void PhysicsSystem::Shutdown()
{

}

void PhysicsSystem::RegisterComponent(PhysicsComponent * comp)
{
  mComponents_.push_back(comp);
}

void PhysicsSystem::RegisterComponent(RigidbodyComponent * comp)
{
  mComponents_.push_back(comp);
  mRigidbodies_.push_back(comp);
}

void PhysicsSystem::RegisterComponent(BoxColliderComponent * comp)
{
  mComponents_.push_back(comp);
  mBoxColliders_.push_back(comp);
}

void PhysicsSystem::RegisterComponent(SphereColliderComponent * comp)
{
  mComponents_.push_back(comp);
  mSphereColliders_.push_back(comp);
}