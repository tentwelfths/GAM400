#pragma once
#include "GameLogicComponent.h"

class DemoController : public GameLogicComponent
{
  public:
    DemoController();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
  private:
    float speed;
};