#include <Box2D\Box2D.h>
#include "MyListen.h"

void MyListen::BeginContact(b2Contact* contact)
{
  b2Fixture* fixtureA = contact->GetFixtureA();
  b2Fixture* fixtureB = contact->GetFixtureB();
  b2Body* bodyA = fixtureA->GetBody();
  b2Body* bodyB = fixtureB->GetBody();
  objOne = static_cast<std::string*>(bodyA->GetUserData());
  objTwo = static_cast<std::string*>(bodyB->GetUserData());
}

void MyListen::EndContact(b2Contact* contact)
{

}