#pragma once
#include "GameLogicComponent.h"

class RandomGen : public GameLogicComponent
{
  public:
    RandomGen();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
  private:
    int numberOne;
    int numberTwo;
    int numberThree;
    int numberFour;
    int numberFive;
};