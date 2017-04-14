#pragma once
#include "GameLogicComponent.h"

class PuzzleHints : public GameLogicComponent
{
  public:
    PuzzleHints();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void SetHintRevealed(int hintNum);
    bool GetHintRevealed(int hintNum);
    int GetValue(int valuePos);
  private:
    std::vector<int> hintsList;
    std::vector<Object*> hintObjects;
    std::string mHintFilenames[5][10];
    std::vector<bool> hintsRevealed;
};