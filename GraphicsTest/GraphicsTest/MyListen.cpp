#include <Box2D\Box2D.h>
#include "MyListen.h"
#include "BoxColliderComponent.h"

void MyListen::BeginContact(b2Contact* contact)
{
  b2Fixture* fixtureA = contact->GetFixtureA();
  b2Fixture* fixtureB = contact->GetFixtureB();
  b2Body* bodyA = fixtureA->GetBody();
  b2Body* bodyB = fixtureB->GetBody();
  objOne = static_cast<BoxColliderComponent*>(bodyA->GetUserData());
  objTwo = static_cast<BoxColliderComponent*>(bodyB->GetUserData());
  objOne->SetJustCollided(true);
  objOne->SetIsColliding(true);
  objTwo->SetJustCollided(true);
  objTwo->SetIsColliding(true);
}

void MyListen::EndContact(b2Contact* contact)
{
  b2Fixture* fixtureA = contact->GetFixtureA();
  b2Fixture* fixtureB = contact->GetFixtureB();
  b2Body* bodyA = fixtureA->GetBody();
  b2Body* bodyB = fixtureB->GetBody();
  objOne = static_cast<BoxColliderComponent*>(bodyA->GetUserData());
  objTwo = static_cast<BoxColliderComponent*>(bodyB->GetUserData());
  objOne->SetIsColliding(false);
  objTwo->SetIsColliding(false);
}