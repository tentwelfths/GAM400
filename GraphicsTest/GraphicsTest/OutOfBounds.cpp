#include "OutOfBounds.h"
#include "TransformComponent.h"
#include "Object.h"

OutOfBounds::OutOfBounds() : GameLogicComponent(GameLogicType::OUTOFBOUNDS), deathHeight(-10.0f)
{
  AddMember(OutOfBounds, deathHeight);
  mName_ = "OutOfBounds";
}

bool OutOfBounds::Initialize()
{
  return true;
}

void OutOfBounds::Update(double dt)
{
  auto* trans = mParent_->GetComponent(TransformComponent);
  if (trans->mPositionY() < deathHeight)
  {
    mParent()->Destroy();
  }
}

void OutOfBounds::Shutdown()
{

}