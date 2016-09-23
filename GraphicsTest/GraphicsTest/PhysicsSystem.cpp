#include <vector>
#include "PhysicsSystem.h"
#include "Object.h"
#include "PhysicsComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "TransformComponent.h"
#include "Collision.h"


PhysicsSystem::PhysicsSystem() : mPhysicsTree(0, Quad(100.0f,100.0f, 0.0f, 0.0f))
{
  mPhysicsTree.setActive(true);
  for (int i = 0; i < MAXLEVELS; ++i)
  {
    mPhysicsTree.createChildren();
  }
  mName_ = "PhysicsSystem";
}

bool PhysicsSystem::Initialize()
{
  return true;
}

void PhysicsSystem::Update(double dt)
{
  mPhysicsTree.clear();
  for (unsigned i = 0; i < mComponents_.size(); ++i)
  {
    auto iter = mComponents_[i];
    if (iter->mParent()->dead)
    {
      mComponents_[i]->clean = true;
      mComponents_.erase(mComponents_.begin() + i);
      --i;
      continue;
    }
  }
  for (unsigned i = 0; i < mRigidbodies_.size(); ++i)
  {
    auto iter = mRigidbodies_[i];
    if (iter->mParent()->dead)
    {
      mRigidbodies_[i]->clean = true;
      mRigidbodies_.erase(mRigidbodies_.begin() + i);
      --i;
      continue;
    }
  }
  for (unsigned i = 0; i < mBoxColliders_.size(); ++i)
  {
    auto iter = mBoxColliders_[i];
    if (iter->mParent()->dead)
    {
      mBoxColliders_[i]->clean = true;
      mBoxColliders_.erase(mBoxColliders_.begin() + i);
      --i;
      continue;
    }
  }
  for (unsigned i = 0; i < mSphereColliders_.size(); ++i)
  {
    auto iter = mSphereColliders_[i];
    if (iter->mParent()->dead)
    {
      mSphereColliders_[i]->clean = true;
      mSphereColliders_.erase(mSphereColliders_.begin() + i);
      --i;
      continue;
    }
  }
  for (auto & iter : mRigidbodies_)
  {
    iter->Update(dt);
    std::vector<Object> collisions;
    mPhysicsTree.insert(*iter->mParent());
  }
  for (auto & iter : mBoxColliders_)
  {
    std::vector<Object> colCheck;
    //mPhysicsTree.retreive(colCheck, *iter->mParent());
    for (auto & iter2 : colCheck)
    {
      auto firstTrans = iter->mParent()->GetComponentA<TransformComponent>("TransformComponent");
      auto secondTrans = iter2.GetComponentA<TransformComponent>("TransformComponent");
      auto firstBox = iter->mParent()->GetComponentA<BoxColliderComponent>("BoxColliderComponent");
      auto secondBox = iter2.GetComponentA<BoxColliderComponent>("BoxColliderComponent");
      

      auto firstPos = firstTrans->mPosition() + firstBox->GetOffset();
      auto secondPos = secondTrans->mPosition() + secondBox->GetOffset();
      
      float firstRight = firstPos.x + firstBox->GetHalfSize().x;
      float firstLeft = firstPos.x - firstBox->GetHalfSize().x;
      float firstTop = firstPos.y + firstBox->GetHalfSize().y;
      float firstBot = firstPos.y - firstBox->GetHalfSize().y;
      
      float secondRight = secondPos.x + secondBox->GetHalfSize().x;
      float secondLeft = secondPos.x - secondBox->GetHalfSize().x;
      float secondTop = secondPos.y + secondBox->GetHalfSize().y;
      float secondBot = secondPos.y - secondBox->GetHalfSize().y;
      
      bool topCheck = firstTop >= secondBot && firstTop <= secondTop;
      bool botCheck = firstBot <= secondTop && firstBot >= secondBot;
      
      bool rightCheck = firstRight >= secondLeft && firstRight <= secondRight;
      bool leftCheck = firstLeft <= secondRight && firstLeft >= secondLeft;
      
      if ((topCheck || botCheck) && ((rightCheck || leftCheck)))
      {
        Collision col;
        //col.AddObjects(*firstTrans->mParent(), *secondTrans->mParent());
        //mCollision_.push_back(col);
      }
    }
  }
//  for (auto & iter = mBoxColliders_.begin(); iter != mBoxColliders_.end(); ++iter)
//  {
//    (*iter)->Update(dt);
//    
//    for (auto & iter2 = iter+1; iter2 != mBoxColliders_.end(); ++iter2)
//    {
//      if ((*iter) == (*iter2))
//      {
//        break;
//      }
//      auto test = mBoxColliders_.back();
//      auto firstTrans = (*iter)->mParent()->GetComponentA<TransformComponent>("TransformComponent");
//      auto secondTrans = (*iter2)->mParent()->GetComponentA<TransformComponent>("TransformComponent");
//      auto firstBox = (*iter)->mParent()->GetComponentA<BoxColliderComponent>("BoxColliderComponent");
//      auto secondBox = (*iter2)->mParent()->GetComponentA<BoxColliderComponent>("BoxColliderComponent");
//
//      auto firstPos = firstTrans->mPosition() + firstBox->GetOffset();
//      auto secondPos = secondTrans->mPosition() + secondBox->GetOffset();
//      
//      float firstRight = firstPos.x + firstBox->GetHalfSize().x;
//      float firstLeft = firstPos.x - firstBox->GetHalfSize().x;
//      float firstTop = firstPos.y + firstBox->GetHalfSize().y;
//      float firstBot = firstPos.y - firstBox->GetHalfSize().y;
//
//      float secondRight = secondPos.x + secondBox->GetHalfSize().x;
//      float secondLeft = secondPos.x - secondBox->GetHalfSize().x;
//      float secondTop = secondPos.y + secondBox->GetHalfSize().y;
//      float secondBot = secondPos.y - secondBox->GetHalfSize().y;
//
//      bool topCheck = firstTop >= secondBot && firstTop <= secondTop;
//      bool botCheck = firstBot <= secondTop && firstBot >= secondBot;
//
//      bool rightCheck = firstRight >= secondLeft && firstRight <= secondRight;
//      bool leftCheck = firstLeft <= secondRight && firstLeft >= secondLeft;
//
//      if ((topCheck || botCheck) && ((rightCheck || leftCheck)))
//      {
//        Collision col;
//        col.AddObjects(firstTrans->mParent(), secondTrans->mParent());
//        mCollision_.push_back(col);
//      }
//    }
//    for (auto iter2 : mSphereColliders_)
//    {
//      auto firstTrans = (*iter)->mParent()->GetComponentA<TransformComponent>("TransformComponent");
//      auto secondTrans = iter2->mParent()->GetComponentA<TransformComponent>("TransformComponent");
//      auto firstBox = (*iter)->mParent()->GetComponentA<BoxColliderComponent>("BoxColliderComponent");
//      auto secondSphere = iter2->mParent()->GetComponentA<SphereColliderComponent>("SphereColliderComponent");
//
//      auto firstPos = firstTrans->mPosition() + firstBox->GetOffset();
//      auto secondPos = secondTrans->mPosition() + secondSphere->GetOffset();
//
//      vec3 copyPos = secondPos;
//
//      if (copyPos.x > firstPos.x + firstBox->GetHalfSize().x)
//      {
//        copyPos.x = firstPos.x + firstBox->GetHalfSize().x;
//      }
//      else if (copyPos.x < firstPos.x - firstBox->GetHalfSize().x)
//      {
//        copyPos.x = firstPos.x - firstBox->GetHalfSize().x;
//      }
//      if (copyPos.y > firstPos.y + firstBox->GetHalfSize().y)
//      {
//        copyPos.y = firstPos.y + firstBox->GetHalfSize().y;
//      }
//      else if (copyPos.y < firstPos.y - firstBox->GetHalfSize().y)
//      {
//        copyPos.y = firstPos.y - firstBox->GetHalfSize().y;
//      }
//      float dist = ((copyPos.x + secondPos.x) * (copyPos.x + secondPos.x)) + ((copyPos.y + secondPos.y) * (copyPos.y + secondPos.y));
//      if (dist < secondSphere->GetRadius())
//      {
//        Collision col;
//        col.AddObjects(firstTrans->mParent(), secondTrans->mParent());
//        mCollision_.push_back(col);
//      }
//    }
//  }
//  for (auto & iter = mSphereColliders_.begin(); iter != mSphereColliders_.end(); ++iter)
//  {
//    (*iter)->Update(dt);
//    for (auto & iter2 = mSphereColliders_.begin(); iter2 != mSphereColliders_.end(); ++iter2)
//    {
//      float r = (*iter)->GetRadius() + (*iter2)->GetRadius();
//      r *= r;
//      auto firstTrans = (*iter)->mParent()->GetComponentA<TransformComponent>("TransformComponent");
//      auto secondTrans = (*iter2)->mParent()->GetComponentA<TransformComponent>("TransformComponent");
//      auto firstSphere = (*iter)->mParent()->GetComponentA<SphereColliderComponent>("SphereColliderComponent");
//      auto secondSphere = (*iter2)->mParent()->GetComponentA<SphereColliderComponent>("SphereColliderComponent");
//
//      auto firstPos = firstTrans->mPosition() + firstSphere->GetOffset();
//      auto secondPos = secondTrans->mPosition() + secondSphere->GetOffset();
//
//      float x = (firstPos.x + secondPos.x) * (firstPos.x + secondPos.x);
//      float y = (firstPos.y + secondPos.y) * (firstPos.y + secondPos.y);
//
//      if (r < (x + y))
//      {
//        Collision col;
//        col.AddObjects(firstTrans->mParent(), secondTrans->mParent());
//        mCollision_.push_back(col);
//      }
//    }
//  }
//  for (auto & iter : mCollision_)
//  {
//    iter.ResolveCollision();
//  }
  std::cout << "Got " << mRigidbodies_.size() << "objects" << std::endl;
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