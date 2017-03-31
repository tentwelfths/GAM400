#pragma once
#include "GameLogicComponent.h"

class KeypadButtonComponent : public GameLogicComponent
{
public:
  KeypadButtonComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  char mDigit;
private:
  Object * manager;
};