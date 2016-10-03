#include <Box2D\Box2D.h>
#include "BoxColliderComponent.h"
#include "PhysicsSystem.h"
#include "Core.h"
#include "Globals.h"
#include "Object.h"

#define EPIFORTRANS 0.00001

BoxColliderComponent::BoxColliderComponent() : PhysicsComponent(PhysicsType::BOXCOLLIDER)
{
  AddMember(BoxColliderComponent, mHalfSize);
  AddMember(BoxColliderComponent, mOffset);
  AddMember(BoxColliderComponent, dynamicState);
  AddMember(BoxColliderComponent, mDensity);
  AddMember(BoxColliderComponent, mFriction);
  PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  g->RegisterComponent(this);
  
  mName_ = "BoxColliderComponent";
}

bool BoxColliderComponent::Initialize()
{
  
  auto* trans = mParent_->GetComponent(TransformComponent);
  PhysicsSystem* g = gCore->GetSystem(PhysicsSystem);
  type = new b2BodyDef();
  box = new b2PolygonShape();
  type->userData = static_cast<void*>(&mParent_->name);
  type->type = b2_dynamicBody;
  type->position.Set(trans->mPositionX() + mOffset.x, trans->mPositionY() + mOffset.y);
  theBody = g->AddBody(type);
  box->SetAsBox(mHalfSize.x, mHalfSize.y);
  b2FixtureDef fixing;
  fixing.shape = box;
  fixing.density = mDensity;
  fixing.friction = mFriction;
  theBody->CreateFixture(&fixing);
  //theBody->SetLinearVelocity(b2Vec2(0.0f,0.0f));
  return true;
}

void BoxColliderComponent::Update(double dt)
{
  auto* trans = mParent_->GetComponent(TransformComponent);
  
  vec3 newPos;
  newPos.x = theBody->GetPosition().x - mOffset.x;
  newPos.y = theBody->GetPosition().y - mOffset.y;
  newPos.z = trans->mPositionZ();
  if (!(newPos.x < trans->mPositionX() + EPIFORTRANS && newPos.x > trans->mPositionX() - EPIFORTRANS) || !(newPos.y < trans->mPositionY() + EPIFORTRANS && newPos.y > trans->mPositionY() - EPIFORTRANS))
  {
    trans->mPosition(newPos);
  }
}

void BoxColliderComponent::Shutdown()
{
  delete type;
  delete box;
}