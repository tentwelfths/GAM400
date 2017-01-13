#pragma once
#include "GameLogicComponent.h"

class PuzzleKnobNode : public GameLogicComponent
{
public:
  PuzzleKnobNode();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  int targetFrequency_;
  int range_;
};