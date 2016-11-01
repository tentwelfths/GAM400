#pragma once
#include "GameLogicComponent.h"

class ConeComponent : public GameLogicComponent
{
public:
  ConeComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  Object* thePlayer;
};