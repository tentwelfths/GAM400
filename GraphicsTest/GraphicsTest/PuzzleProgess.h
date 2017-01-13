#pragma once
#include "GameLogicComponent.h"

class PuzzleProgress : public GameLogicComponent
{
  public:
    PuzzleProgress();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void Failed();
    void Success();
  private:
    int targetForRound_;
    int curr_;
};