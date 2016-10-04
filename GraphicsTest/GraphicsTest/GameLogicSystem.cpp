#include "GameLogicSystem.h"
#include "GameLogicComponent.h"
#include "Core.h"
#include "Globals.h"
#include "Object.h"

GameLogicSystem::GameLogicSystem()
{
  mName_ = "GameLogicSystem";
  wasEditor = gCore->editor;
}

bool GameLogicSystem::Initialize()
{
  return true;
}

void GameLogicSystem::Update(double dt)
{
  if (gCore->editor)wasEditor = true;
  if (gCore->editor)return;
  for (unsigned i = 0; i < mComponents_.size(); ++i)
  {
    auto iter = mComponents_[i];
    if (iter->mParent()->dead)
    {
      mComponents_[i]->clean = true;
      mComponents_.erase(mComponents_.begin() + i);
      --i;
      continue;
    }
  }
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