#include "Core.h"
#include "GraphicsSystem.h"
#include "NetworkingSystem.h"
#include "FramerateController.h"
#include "GameLogicSystem.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "PhysicsSystem.h"

void Core::RegisterSystem(System * s)
{
  mSystems.insert({ s->mName(), s });
  mSystemsOrdered.push_back(s);
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
  auto * o = new ObjectSystem();
  RegisterSystem(o);
  auto * p = new PhysicsSystem();
  RegisterSystem(p);
  auto * i = new InputSystem();
  RegisterSystem(i);

  for (auto iter = mSystems.begin(); iter != mSystems.end(); ++iter)
    (*iter).second->Initialize();

  g->LoadTexture("Kakka_Carrot_Veggie.png");
  return true;
}

void Core::Update(double dt)
{
  auto * frc = static_cast<FramerateController*>(mSystems["FramerateController"]);
  frc->StartFrame();
  for (auto & iter : mSystemsOrdered)
  {
    iter->Update(frc->GetDT());
  }
  frc->EndFrame();
}

void Core::Shutdown()
{

}
