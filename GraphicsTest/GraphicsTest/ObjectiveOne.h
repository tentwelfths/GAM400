#pragma once
#include "GameLogicComponent.h"

class ObjectiveOne : public GameLogicComponent
{
public:
  ObjectiveOne();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  Object* zone;
  std::string zoneName;
};