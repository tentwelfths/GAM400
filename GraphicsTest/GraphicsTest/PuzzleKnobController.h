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
  private:
    int frequency;
    int controllerID_;
};