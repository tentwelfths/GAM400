#pragma once

#include "GameLogicComponent.h"

class ControllerCameraFollow : public GameLogicComponent
{
public:
  ControllerCameraFollow();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
};