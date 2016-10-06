#pragma once
#include "GameLogicComponent.h"

class DemoReload : public GameLogicComponent
{
public:
  DemoReload();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  float timeToUnload;
  float time;
};