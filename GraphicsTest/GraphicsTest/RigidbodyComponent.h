#pragma once
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

//class RigidbodyComponent : public PhysicsComponent
//{
//public:
//  RigidbodyComponent();
//  bool Initialize();
//  void Update(double dt);
//  void Shutdown();
//  void SetLocked(bool newLocked);
//  void SwapLocked() { mRotationLocked = !mRotationLocked; }
//  void SetMass(float newMass);
//  void SetVel(float x, float y, float z);
//  void SetVel(vec3 newVel);
//  void SetState(DynamicState newState);
//  void AddVel(float x, float y, float z);
//  bool GetLocked();
//  float GetMass();
//  vec3 GetVel();
//  DynamicState GetState();
//  
//private:
//  bool mRotationLocked;
//  float mMass;
//  vec3 mVelocity;
//  DynamicState mDynamic;
//};