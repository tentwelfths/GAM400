#pragma once
#include "GameLogicComponent.h"

class PuzzleSwitchNode : public GameLogicComponent
{
public:
  PuzzleSwitchNode();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  int targetValue_;
};