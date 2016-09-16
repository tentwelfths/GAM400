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
      
      if (((firstPos.x - firstBox->GetHalfSize().x) < (secondPos.x + secondBox->GetHalfSize().x)) &&
        ((firstPos.x + firstBox->GetHalfSize().x) > (secondPos.x - secondBox->GetHalfSize().x)) &&
        ((firstPos.y - firstBox->GetHalfSize().y) < (secondPos.y + secondBox->GetHalfSize().y)) &&
        ((firstPos.y + firstBox->GetHalfSize().y) > (secondPos.y - secondBox->GetHalfSize().y)))
      {
        Collision col;
        col.AddObjects(firstTrans->mParent(), secondTrans->mParent());
        mCollision_.push_back(col);
      }
    }
    for (auto & iter2 : mSphereColliders_)
    {
      auto firstTrans = iter->mParent()->GetComponentA<TransformComponent>("TransformComponent");
      auto secondTrans = iter2->mParent()->GetComponentA<TransformComponent>("TransformComponent");
      auto firstBox = iter->mParent()->GetComponentA<BoxColliderComponent>("BoxColliderComponent");
      auto secondSphere = iter2->mParent()->GetComponentA<SphereColliderComponent>("SphereColliderComponent");

      auto firstPos = firstTrans->mPosition() + firstBox->GetOffset();
      auto secondPos = secondTrans->mPosition() + secondSphere->GetOffset();

      vec3 copyPos = secondPos;

      if (copyPos.x > firstPos.x + firstBox->GetHalfSize().x)
      {
        copyPos.x = firstPos.x + firstBox->GetHalfSize().x;
      }
      else if (copyPos.x < firstPos.x - firstBox->GetHalfSize().x)
      {
        copyPos.x = firstPos.x - firstBox->GetHalfSize().x;
      }
      if (copyPos.y > firstPos.y + firstBox->GetHalfSize().y)
      {
        copyPos.y = firstPos.y + firstBox->GetHalfSize().y;
      }
      else if (copyPos.y < firstPos.y - firstBox->GetHalfSize().y)
      {
        copyPos.y = firstPos.y - firstBox->GetHalfSize().y;
      }
      float dist = ((copyPos.x + secondPos.x) * (copyPos.x + secondPos.x)) + ((copyPos.y + secondPos.y) * (copyPos.y + secondPos.y));
      if (dist < secondSphere->GetRadius())
      {
        Collision col;
        col.AddObjects(firstTrans->mParent(), secondTrans->mParent());
        mCollision_.push_back(col);
      }
    }
  }
  for (auto & iter : mSphereColliders_)
  {
    iter->Update(dt);
    for (auto iter2 = iter; iter2 != mSphereColliders_.back(); ++iter2)
    {
      float r = iter->GetRadius() + iter2->GetRadius();
      r *= r;
      auto firstTrans = iter->mParent()->GetComponentA<TransformComponent>("TransformComponent");
      auto secondTrans = iter2->mParent()->GetComponentA<TransformComponent>("TransformComponent");
      auto firstSphere = iter->mParent()->GetComponentA<SphereColliderComponent>("SphereColliderComponent");
      auto secondSphere = iter2->mParent()->GetComponentA<SphereColliderComponent>("SphereColliderComponent");

      auto firstPos = firstTrans->mPosition() + firstSphere->GetOffset();
      auto secondPos = secondTrans->mPosition() + secondSphere->GetOffset();

      float x = (firstPos.x + secondPos.x) * (firstPos.x + secondPos.x);
      float y = (firstPos.y + secondPos.y) * (firstPos.y + secondPos.y);

      if (r < (x + y))
      {
        Collision col;
        col.AddObjects(firstTrans->mParent(), secondTrans->mParent());
        mCollision_.push_back(col);
      }
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
  mRigidbodies_.push_back(comp);
}

void PhysicsSystem::RegisterComponent(BoxColliderComponent * comp)
{
  mBoxColliders_.push_back(comp);
}

void PhysicsSystem::RegisterComponent(SphereColliderComponent * comp)
{
  mSphereColliders_.push_back(comp);
}