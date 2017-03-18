#pragma once
#include "GameLogicComponent.h"

//enum Hints {NONE, KNOBHINT, SWITCHHINT, DISTANCEHINT, LIGHTHINT, SLIDERHINT};

class PuzzleObject : public GameLogicComponent
{
public:
  PuzzleObject();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void SetHint(int value) { hintValue_ = value; }
private:
  int hintValue_;
  bool active_;
  bool clicked_;
  bool collide_;
};