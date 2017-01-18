#pragma once
#include "GameLogicComponent.h"
class InputSystem;

class PuzzleSwitchController : public GameLogicComponent
{
public:
  PuzzleSwitchController();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  int GetControllerID();
  int GetValue() { return value_; }
private:
  int value_;
  int controllerID_;
};