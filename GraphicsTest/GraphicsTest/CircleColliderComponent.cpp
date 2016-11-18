#include <Box2D\Box2D.h>
#include "CircleColliderComponent.h"
#include "PhysicsSystem.h"
#include "Core.h"
#include "Globals.h"
#include "Object.h"
#include "Messages.h"

CircleColliderComponent::CircleColliderComponent() : PhysicsComponent(PhysicsType::SPHERECOLLIDER), justCollided(false), isColliding(false)
{
  AddMember(CircleColliderComponent, mRadius);
  AddMember(CircleColliderComponent, mOffset);
  AddMember(CircleColliderComponent, dynamicState);
  AddMember(CircleColliderComponent, mDensity);
  AddMember(CircleColliderComponent, mFriction);
  AddMember(CircleColliderComponent, mGhost);
  //PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  //g->RegisterComponent(this);

  mName_ = "CircleColliderComponent";
}

bool CircleColliderComponent::Initialize()
{
  auto* trans = mParent_->GetComponent(TransformComponent);
  PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  type = new b2BodyDef();
  circle = new b2CircleShape();
  type->userData = static_cast<void*>(this);
  type->type = static_cast<b2BodyType>(dynamicState);
  type->position.Set(trans->mPositionX(), trans->mPositionY());
  theBody = g->AddBody(type);
  circle->m_p = b2Vec2(mOffset.x, mOffset.y);
  circle->m_radius = mRadius;
  b2FixtureDef fixing;
  fixing.shape = circle;
  fixing.isSensor = mGhost;
  fixing.density = mDensity;
  fixing.friction = mFriction;
  theBody->CreateFixture(&fixing);
  //theBody->SetLinearVelocity(b2Vec2(0.0f,0.0f));
  return true;
}

void CircleColliderComponent::Update(double dt)
{
  //justCollided = false;
  auto* trans = mParent_->GetComponent(TransformComponent);

  vec3 newPos;
  newPos.x = theBody->GetPosition().x;
  newPos.y = theBody->GetPosition().y;
  newPos.z = trans->mPositionZ();
  if (!(newPos.x < trans->mPositionX() + EPIFORTRANS && newPos.x > trans->mPositionX() - EPIFORTRANS) || !(newPos.y < trans->mPositionY() + EPIFORTRANS && newPos.y > trans->mPositionY() - EPIFORTRANS))
  {
    trans->mPosition(newPos);
  }
}

void CircleColliderComponent::Shutdown()
{
  PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  g->RemoveBody(theBody);
  delete type;
  delete circle;
}

void CircleColliderComponent::ConfirmPosition(){
  auto* trans = mParent_->GetComponent(TransformComponent);
  if (!trans)
  {
    return;
  }
  b2Vec2 testVec(trans->mPositionX(), trans->mPositionY());
  theBody->SetTransform(testVec, trans->mRotationZ());
}