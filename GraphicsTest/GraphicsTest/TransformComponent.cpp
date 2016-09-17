#include "TransformComponent.h"
#include "Object.h"
#include "Globals.h"

TransformComponent::TransformComponent()
{
  AddMember(TransformComponent, mPosition_);
  AddMember(TransformComponent, mScale_);
  AddMember(TransformComponent, mRotation_);
  mName_ = "TransformComponent";
}

bool TransformComponent::Initialize()
{
  return true;
}

void TransformComponent::Update(double dt)
{

}

void TransformComponent::Shutdown()
{

}


void       TransformComponent::mPosition(glm::vec3 pos)             {mParent_->hasChanged = true; mPosition_ = pos; }
void       TransformComponent::mPosition(float x, float y, float z) {mParent_->hasChanged = true; mPosition_ = glm::vec3(x, y, z); }
void       TransformComponent::mPositionX(float x)                  {mParent_->hasChanged = true; mPosition_.x = x; }
void       TransformComponent::mPositionY(float y)                  {mParent_->hasChanged = true; mPosition_.y = y; }
void       TransformComponent::mPositionZ(float z)                  {mParent_->hasChanged = true; mPosition_.z = z; }
void       TransformComponent::mScale(glm::vec3 pos)                {mParent_->hasChanged = true; mScale_ = pos; }
void       TransformComponent::mScale(float x, float y, float z)    {mParent_->hasChanged = true; mScale_ = glm::vec3(x, y, z); }
void       TransformComponent::mScaleX(float x)                     {mParent_->hasChanged = true; mScale_.x = x; }
void       TransformComponent::mScaleY(float y)                     {mParent_->hasChanged = true; mScale_.y = y; }
void       TransformComponent::mScaleZ(float z)                     {mParent_->hasChanged = true; mScale_.z = z; }
void       TransformComponent::mRotation(glm::vec3 pos)             {mParent_->hasChanged = true; mRotation_ = pos; }
void       TransformComponent::mRotation(float x, float y, float z) {mParent_->hasChanged = true; mRotation_ = glm::vec3(x, y, z); }
void       TransformComponent::mRotationX(float x)                  {mParent_->hasChanged = true; mRotation_.x = x; }
void       TransformComponent::mRotationY(float y)                  {mParent_->hasChanged = true; mRotation_.y = y; }
void       TransformComponent::mRotationZ(float z)                  {mParent_->hasChanged = true; mRotation_.z = z; }
glm::vec3  TransformComponent::mPosition(){ return mPosition_; }
float      TransformComponent::mPositionX(){ return mPosition_.x; }
float      TransformComponent::mPositionY(){ return mPosition_.y; }
float      TransformComponent::mPositionZ(){ return mPosition_.z; }
glm::vec3  TransformComponent::mScale(){ return mScale_; }
float      TransformComponent::mScaleX(){ return mScale_.x; }
float      TransformComponent::mScaleY(){ return mScale_.y; }
float      TransformComponent::mScaleZ(){ return mScale_.z; }
glm::vec3  TransformComponent::mRotation()                   { return mRotation_; }
float      TransformComponent::mRotationX()                  { return mRotation_.x; }
float      TransformComponent::mRotationY()                  { return mRotation_.y; }
float      TransformComponent::mRotationZ()                  { return mRotation_.z; }