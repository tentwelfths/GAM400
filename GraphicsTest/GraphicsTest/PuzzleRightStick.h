#pragma once
#include "GameLogicComponent.h"

class PuzzleRightStick : public GameLogicComponent
{
public:
  PuzzleRightStick();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  
};