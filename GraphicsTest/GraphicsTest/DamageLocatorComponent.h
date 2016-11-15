#pragma once
#include "GameLogicComponent.h"

class Object;
#define NUMARROWS 5

class DamageLocatorComponent : public GameLogicComponent
{
public:
  DamageLocatorComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void SetArrowVisibility(bool vis[5]);
private:
  Object * arrows[NUMARROWS];
  char count;
  bool arrowVis[NUMARROWS];
};