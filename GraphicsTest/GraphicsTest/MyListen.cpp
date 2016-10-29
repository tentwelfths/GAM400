#include <Box2D\Box2D.h>
#include "MyListen.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Globals.h"
#include "Object.h"
#include "PhysicsSystem.h"
#include "Messages.h"
#include "MessagingSystem.h"

void MyListen::BeginContact(b2Contact* contact)
{
  b2Fixture* fixtureA = contact->GetFixtureA();
  b2Fixture* fixtureB = contact->GetFixtureB();
  b2Body* bodyA = fixtureA->GetBody();
  b2Body* bodyB = fixtureB->GetBody();
  objOne = static_cast<BoxColliderComponent*>(bodyA->GetUserData());
  objTwo = static_cast<BoxColliderComponent*>(bodyB->GetUserData());

  //CollisionStartedMessage* col = new CollisionStartedMessage();
  //col->obj1 = objOne->mParent();
  //col->obj2 = objTwo->mParent();

  IMessage col(MessageType::COLLISIONSTARTED);
  CollisionStartedMessage * msgData = reinterpret_cast<CollisionStartedMessage *>(col.data);
  msgData->obj1 = objOne->mParent();
  msgData->obj2 = objTwo->mParent();

  MessagingSystem* m = gCore->GetSystem(MessagingSystem);
  m->SendMessageToObject(col, msgData->obj1->ID);
  m->SendMessageToObject(col, msgData->obj2->ID);

  //objOne->SetJustCollided(true);
  //objOne->SetIsColliding(true);
  //objTwo->SetJustCollided(true);
  //objTwo->SetIsColliding(true);
}

void MyListen::EndContact(b2Contact* contact)
{
  b2Fixture* fixtureA = contact->GetFixtureA();
  b2Fixture* fixtureB = contact->GetFixtureB();
  b2Body* bodyA = fixtureA->GetBody();
  b2Body* bodyB = fixtureB->GetBody();
  objOne = static_cast<BoxColliderComponent*>(bodyA->GetUserData());
  objTwo = static_cast<BoxColliderComponent*>(bodyB->GetUserData());

  //CollisionEndedMessage* col = new CollisionEndedMessage();
  //col->obj1 = objOne->mParent();
  //col->obj2 = objTwo->mParent();

  IMessage col(MessageType::COLLISIONENDED);
  CollisionEndedMessage * msgData = reinterpret_cast<CollisionEndedMessage *>(col.data);
  msgData->obj1 = objOne->mParent();
  msgData->obj2 = objTwo->mParent();

  MessagingSystem* m = gCore->GetSystem(MessagingSystem);
  m->SendMessageToObject(col, msgData->obj1->ID);
  m->SendMessageToObject(col, msgData->obj2->ID);
  //objOne->SetIsColliding(false);
  //objTwo->SetIsColliding(false);
}