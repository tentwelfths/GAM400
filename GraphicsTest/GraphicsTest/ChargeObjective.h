#pragma once
#include "GameLogicComponent.h"

class ChargeObjective : public GameLogicComponent
{
public:
  ChargeObjective();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  bool playersOn[4];
  float charge;
  float chargeCap;
};