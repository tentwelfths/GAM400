#include <stdlib.h>
#include "RandomGen.h"

RandomGen::RandomGen() : GameLogicComponent(GameLogicType::RANDOM)
{
  mName_ = "RandomGen";
}

bool RandomGen::Initialize()
{
  numberOne = rand() % 9;
  numberTwo = rand() % 9;
  numberThree = rand() % 9;
  numberFour = rand() % 9;
  numberFive = rand() % 9;
  return true;
}

void RandomGen::Update(double dt)
{

}

void RandomGen::Shutdown()
{
}