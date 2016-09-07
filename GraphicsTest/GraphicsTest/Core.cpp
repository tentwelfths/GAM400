#include "Core.h"
#include "GraphicsSystem.h"
#include "NetworkingSystem.h"

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

  
  for (auto iter = mSystems.begin(); iter != mSystems.end(); ++iter)
    (*iter).second->Initialize();
  g->Update(0);
  g->LoadTexture("A.png");
  g->LoadTexture("Arrow.png");
  g->LoadTexture("DamagePickup.png");
  g->LoadTexture("HealSpell.png");
  g->LoadTexture("Kakka_Carrot_Veggie.png");
  g->LoadTexture("Key.png");
  return true;
}

void Core::Update(double dt)
{
  for (auto & iter : mSystems)
  {
    iter.second->Update(dt);
  }
}

void Core::Shutdown()
{

}
