#pragma once
#include "GameLogicComponent.h"

class PlayerControllerComponent : public GameLogicComponent
{
public:
  PlayerControllerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  float speed;
};
