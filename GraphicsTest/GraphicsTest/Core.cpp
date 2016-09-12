#include "Core.h"
#include "GraphicsSystem.h"
#include "NetworkingSystem.h"
#include "FramerateController.h"
#include "GameLogicSystem.h"
#include "InputSystem.h"
#include "ObjectSystem.h"

void Core::RegisterSystem(System * s)
{
  mSystems.insert({ s->mName(), s });
}

bool Core::Initialize()
{
  auto * g = new GraphicsSystem();
  RegisterSystem(g);
  auto * n = new NetworkingSystem();
  RegisterSystem(n);
  auto * f = new FramerateController();
  RegisterSystem(f);
  auto * l = new GameLogicSystem();
  RegisterSystem(l);
  auto * i = new InputSystem();
  RegisterSystem(i);
  auto * o = new ObjectSystem();
  RegisterSystem(o);
  
  for (auto iter = mSystems.begin(); iter != mSystems.end(); ++iter)
    (*iter).second->Initialize();
  g->Update(0);
  g->LoadTexture("Kakka_Carrot_Veggie.png");
  return true;
}

void Core::Update(double dt)
{
  auto * frc = static_cast<FramerateController*>(mSystems["FramerateController"]);
  frc->StartFrame();
  for (auto & iter : mSystems)
  {
    iter.second->Update(frc->GetDT());
  }
  frc->EndFrame();
}

void Core::Shutdown()
{

}
