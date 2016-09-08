#include "TransformComponent.h"
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
