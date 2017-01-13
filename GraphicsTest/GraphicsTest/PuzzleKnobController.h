#pragma once
#include "GameLogicComponent.h"
class InputSystem;

class PuzzleKnobController : public GameLogicComponent
{
  public:
    PuzzleKnobController();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    int GetControllerID();
    int GetFrequency() { return frequency; }
  private:
    int frequency;
    int controllerID_;
};