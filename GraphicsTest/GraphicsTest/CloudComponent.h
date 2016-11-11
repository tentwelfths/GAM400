#pragma once
#include "GameLogicComponent.h"

class CloudComponent : public GameLogicComponent
{
public:
  CloudComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  float lifeTime;
};