#pragma once
#include "GameLogicComponent.h"

class PuzzleObjectSpawn{
  public:
    PuzzleObjectSpawn();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
  private:
    int numOfObjects;
};