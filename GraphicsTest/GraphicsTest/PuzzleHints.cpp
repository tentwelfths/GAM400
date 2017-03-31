#include "PuzzleHints.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

#define NUMOFHINTS 5

PuzzleHints::PuzzleHints() : GameLogicComponent(GameLogicType::PUZZLEHINTS)
{
  mName_ = "PuzzleHints";
}

bool PuzzleHints::Initialize()
{
  for (int i = 0; i < NUMOFHINTS; ++i)
  {
    hintsList.push_back((rand() % 10));
    hintsRevealed.push_back(false);
  }
  return true;
}

void PuzzleHints::Update(double dt)
{

}

void PuzzleHints::Shutdown()
{

}

void PuzzleHints::SetHintRevealed(int hintNum)
{
  hintsRevealed[hintNum - 1] = true;
  auto * o = gCore->GetSystem(ObjectSystem);
  std::string hintName = "TheHint" + std::to_string(hintNum);
  auto * obj = o->GetFirstItemByName(hintName);
  for (int i = 0; i < 5; ++i)
  {
    obj->mVisibility[i] = true;
  }
  obj->hasChanged = true;
}

bool PuzzleHints::GetHintRevealed(int hintNum)
{
  return hintsRevealed[hintNum];
}