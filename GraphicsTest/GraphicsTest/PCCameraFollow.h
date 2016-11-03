#pragma once

#include "GameLogicComponent.h"

class PCCameraFollow : public GameLogicComponent
{
public:
  PCCameraFollow();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
};