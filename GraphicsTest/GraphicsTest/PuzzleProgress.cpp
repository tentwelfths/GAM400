#include "PuzzleProgess.h"
#include "PuzzlePulse.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

PuzzleProgress::PuzzleProgress() : GameLogicComponent(GameLogicType::PROGRESS), targetForRound_(5), curr_(0)
{
  AddMember(PuzzleProgress, targetForRound_);
  mName_ = "PuzzleProgress";
}

bool PuzzleProgress::Initialize()
{
  return true;
}

void PuzzleProgress::Update(double dt)
{

}

void PuzzleProgress::Shutdown()
{

}

void PuzzleProgress::Failed()
{
  curr_ = 0;
}
void PuzzleProgress::Success()
{
  curr_ += 1;
  if (curr_ == targetForRound_)
  {
    auto * o = gCore->GetSystem(ObjectSystem);
    auto * pulseObj = o->GetFirstItemByName("Pulse");
    auto * pulse = pulseObj->GetComponent(PuzzlePulse);
    pulse->SetSpawn(-6, pulseObj->GetComponent(TransformComponent)->mPositionY() - 1);
    targetForRound_ += 1;
  }
}