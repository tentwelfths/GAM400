#include "PuzzleHints.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "JSONTranslator.h"

#define NUMOFHINTS 5

PuzzleHints::PuzzleHints() : GameLogicComponent(GameLogicType::PUZZLEHINTS)
{
  mName_ = "PuzzleHints";
}

bool PuzzleHints::Initialize()
{
  JSONTranslator j;

  for (int i = 0; i < 10; ++i)
    mHintFilenames[0][i] = "DistanceHint" +std::to_string(i)+ ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[1][i] = "KnobHint" + std::to_string(i) + ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[2][i] = "PhotocellHint" + std::to_string(i) + ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[3][i] = "SliderHint" + std::to_string(i) + ".png";
  for (int i = 0; i < 10; ++i)
    mHintFilenames[4][i] = "SwitchesHint" + std::to_string(i) + ".png";


  for (int i = 0; i < NUMOFHINTS; ++i)
  {
    int index = (rand() % 10);
    hintsList.push_back(index);
    Object * obj = j.CreateObjectFromFile("Empty.json");
    obj->GetComponent(SpriteComponent)->SetTexture(mHintFilenames[4][7]);
    obj->GetComponent(SpriteComponent)->mTint({ 0, 1, 0 });
    glm::vec3 newTrans(-7, 34 - 2 * i, 4);
    obj->GetComponent(TransformComponent)->mPosition(newTrans);
    obj->GetComponent(TransformComponent)->mScale(8,2,1);
    obj->save = false;
    obj->Initialize();
    obj->Register();
    hintObjects.push_back(obj);
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
  for (int i = 0; i < 5; ++i)
  {
    hintObjects[hintNum-1]->mVisibility[i] = true;
  }
  hintObjects[hintNum - 1]->hasChanged = true;
}

bool PuzzleHints::GetHintRevealed(int hintNum)
{
  return hintsRevealed[hintNum];
}

int PuzzleHints::GetValue(int valuePos)
{
  return hintsList[valuePos];
}