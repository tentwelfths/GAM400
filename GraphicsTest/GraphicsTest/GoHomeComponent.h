#pragma once
#include "GameLogicComponent.h"

class PlayerControllerComponent;

class GoHomeComponent : public GameLogicComponent
{
public:
  GoHomeComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  float mCountDown;
  float mTarget;
};