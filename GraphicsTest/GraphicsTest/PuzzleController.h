#pragma once
#include "GameLogicComponent.h"
class InputSystem;

class PuzzleController : public GameLogicComponent
{
  public:
    PuzzleController();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
  private:
    float speed_;
};