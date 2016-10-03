#include <Box2D\Box2D.h>
#include <vector>
#include "PhysicsSystem.h"
#include "Object.h"
#include "PhysicsComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "TransformComponent.h"
#include "MyListen.h"
#define VECPASS 8
#define POSPASS 3

PhysicsSystem::PhysicsSystem()
{
  b2Vec2 grav(0.0f, 0.0f);
  theWorld = new b2World(grav);
  theListen = new MyListen();
  theWorld->SetContactListener(theListen);
  mName_ = "PhysicsSystem";
}

bool PhysicsSystem::Initialize()
{
  return true;
}

void PhysicsSystem::Update(double dt)
{
  theWorld->Step(dt, VECPASS, POSPASS);
  for (auto & iter : mComponents_)
  {
    iter->Update(dt);
  }
}

void PhysicsSystem::Shutdown()
{
  delete theWorld;
  delete theListen;
}

void PhysicsSystem::RegisterComponent(PhysicsComponent * comp)
{
  mComponents_.push_back(comp);
}

b2Body* PhysicsSystem::AddBody(b2BodyDef* theBody)
{
  return theWorld->CreateBody(theBody);
}