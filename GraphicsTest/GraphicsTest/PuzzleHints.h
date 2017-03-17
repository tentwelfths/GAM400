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
  private:
    std::vector<int> hintsList;
    std::vector<bool> hintsRevealed;
};