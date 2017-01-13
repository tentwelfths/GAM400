#include <Box2D\Box2D.h>
#include "PuzzlePulse.h"
#include "PuzzleProgess.h"
#include "ObjectSystem.h"
#include "BoxColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

PuzzlePulse::PuzzlePulse() : GameLogicComponent(GameLogicType::PUZZLEPULSE), x_(0), y_(0)
{
  AddMember(PuzzlePulse, x_);
  AddMember(PuzzlePulse, y_);
  mName_ = "PuzzlePulse";
}

bool PuzzlePulse::Initialize()
{
  return true;
}

void PuzzlePulse::Update(double dt)
{
  auto * box = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 moveVec(5.0f, 0.0f);
  auto * theBody = box->GetBody();
  theBody->SetLinearVelocity(moveVec);
  if (theBody->GetPosition().x > 10)
  {
    Restart();
    auto * o = gCore->GetSystem(ObjectSystem);
    auto * progress = o->GetFirstItemByName("Tracker")->GetComponent(PuzzleProgress);
    progress->Failed();
  }
}

void PuzzlePulse::Shutdown()
{

}

void PuzzlePulse::SetSpawn(float x, float y)
{
  x_ = x;
  y_ = y;
}

void PuzzlePulse::Restart()
{
  b2Vec2 set(x_, y_);
  auto * box = mParent_->GetComponent(BoxColliderComponent);
  auto * theBody = box->GetBody();
  theBody->SetTransform(set, 0);
}