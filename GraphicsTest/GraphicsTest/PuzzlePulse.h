#pragma once
#include "GameLogicComponent.h"

class PuzzlePulse : public GameLogicComponent
{
  public:
    PuzzlePulse();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void SetSpawn(float x, float y);
    void Restart();
  private:
    float x_;
    float y_;
};