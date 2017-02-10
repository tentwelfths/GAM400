#pragma once
#include "GameLogicComponent.h"

class ChangeSpriteComponent : public GameLogicComponent
{
public:
  ChangeSpriteComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  std::string changeTo;
};