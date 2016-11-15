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
  bool done;
  float charge;
  float chargeCap;
};