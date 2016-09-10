#include "GameLogicComponent.h"
#include "GameLogicSystem.h"
#include "Core.h"
#include "Globals.h"


GameLogicComponent::GameLogicComponent(GameLogicType t)
{
  GameLogicSystem* g = gCore->GetSystem(GameLogicSystem);
  g->RegisterComponent(this);

  mName_ = "GameLogicComponent";
  mType_ = t;
}

bool GameLogicComponent::Initialize()
{
  return true;
}

void GameLogicComponent::Update(double dt)
{
}

void GameLogicComponent::Shutdown()
{
}