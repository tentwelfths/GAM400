#include "GameLogicSystem.h"
#include "GameLogicComponent.h"

GameLogicSystem::GameLogicSystem()
{
  mName_ = "GameLogicSystem";
}

bool GameLogicSystem::Initialize()
{
  return true;
}

void GameLogicSystem::Update(double dt)
{
  for (auto & iter : mComponents_)
  {
    iter->Update(dt);
  }
}

void GameLogicSystem::Shutdown()
{

}

void GameLogicSystem::RegisterComponent(GameLogicComponent * comp)
{
  mComponents_.push_back(comp);
}