#pragma once
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

class PhysicsMaterialComponent : public PhysicsComponent
{
public:
  PhysicsMaterialComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void SetPriority(bool newPriority);
  void SwapPriority() { mHighPriority = !mHighPriority; }
  void SetRestitution(float newRestitution);
  void SetFriction(float newFriction);
  bool GetPriority();
  float GetRestitution();
  float GetFriction();

private:
  bool mHighPriority;
  float mRestitution;
  float mFriction;
};