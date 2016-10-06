#include "DemoReload.h"
#include "Core.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "Object.h"

DemoReload::DemoReload() : GameLogicComponent(GameLogicType::RELOAD), timeToUnload(1.0f), time(0.0f)
{
  mName_ = "DemoReload";
}

bool DemoReload::Initialize()
{
  return true;
}

void DemoReload::Update(double dt)
{
  auto * checker = gCore->GetSystem(ObjectSystem);
  auto player = checker->GetFirstItemByName("Player");
  if (!player)
  {
    if (time >= timeToUnload)
    {
      gCore->UnloadLevel();
      gCore->LoadLevel("DemoLevel.json");
      time = 0.0f;
    }
    time += dt;
  }
}

void DemoReload::Shutdown()
{

}