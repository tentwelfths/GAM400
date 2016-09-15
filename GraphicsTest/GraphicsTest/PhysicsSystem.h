#pragma once
#include "System.h"
#include "Standard.h"

class PhysicsComponent;
class RigidbodyComponent;
class BoxColliderComponent;
class SphereColliderComponent;
class Collision;

enum class DynamicState { STATIC, DYNAMIC, KINEMATIC };

class PhysicsSystem : public System
{
public:
  PhysicsSystem();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void RegisterComponent(PhysicsComponent * comp);
  void RegisterComponent(RigidbodyComponent * comp);
  void RegisterComponent(BoxColliderComponent * comp);
  void RegisterComponent(SphereColliderComponent * comp);
private:
  std::vector<PhysicsComponent *> mComponents_;
  std::vector<RigidbodyComponent *> mRigidbodies_;
  std::vector<BoxColliderComponent *> mBoxColliders_;
  std::vector<SphereColliderComponent *> mSphereColliders_;
  std::vector<Collision> mCollision_;
};