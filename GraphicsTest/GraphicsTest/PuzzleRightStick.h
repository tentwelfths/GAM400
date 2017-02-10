#pragma once
#include "GameLogicComponent.h"

enum PuzzleArrow {UP, DOWN, RIGHT, LEFT};

class PuzzleRightStick : public GameLogicComponent
{
public:
  PuzzleRightStick();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void RandomArrow();
  int Completed() { return completed_; }
private:
  float timeLeft_;
  float timeReset_;
  PuzzleArrow theArrow_;
  int completed_;
};