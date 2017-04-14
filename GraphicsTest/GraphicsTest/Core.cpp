#include "Core.h"
#include "GraphicsSystem.h"
#include "NetworkingSystem.h"
#include "FramerateController.h"
#include "GameLogicSystem.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "PhysicsSystem.h"
#include "EditorSystem.h"
#include "JSONTranslator.h"
#include "AudioSystem.h"
#include <time.h>

void Core::RegisterSystem(System * s)
{
  mSystems.insert({ s->mName(), s });
  mSystemsOrdered.push_back(s);
}

bool Core::Initialize()
{
  srand(time(NULL));
  editor = false;
  active = true;
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
  auto * e = new EditorSystem();
  RegisterSystem(e);
  auto * i = new InputSystem();
  RegisterSystem(i);
  auto * a = new AudioSystem();
  RegisterSystem(a);

  for (auto iter = mSystems.begin(); iter != mSystems.end(); ++iter)
    (*iter).second->Initialize();
  srand(f->GetFrameStart());
  //g->LoadTexture("../Assets/Kakka_Carrot_Veggie.png");
  //g->LoadTexture("../Assets/Key.png");
  //g->LoadTexture("../Assets/player.png");
  //g->LoadTexture("../Assets/bolt.png");
  //g->LoadTexture("../Assets/rock.png");
  LoadTextures("../Assets/Textures.JSON", g);
  unloading = false;
  needToLoad = "";
  return true;
}

void Core::LoadTextures(std::string filename, GraphicsSystem * g)
{
  
  JSONTranslator j;
  j.LoadTextures(filename, g);
}

void Core::Update(double dt)
{


  auto * frc = static_cast<FramerateController*>(mSystems["FramerateController"]);
  frc->StartFrame();
  if (unloading){
    unloading = false;
  }
  if (needToLoad != ""){
    LoadLevel(needToLoad);
  }
  for (auto & iter : mSystemsOrdered)
  {
    frc->SetSystemStart(iter->mName());
    iter->Update(frc->GetDT());
    frc->SetSystemEnd();
  }

  //frc->PrintSystemTimeAnalysis();
  frc->EndFrame();
}

void Core::Shutdown()
{

}
void Core::LoadLevel(std::string filename){
  needToLoad = filename;
  currentLevel = filename;
  if (unloading)return;
  JSONTranslator j;
  j.LoadLevelFromFile(needToLoad);
  needToLoad = "";

}
void Core::UnloadLevel(){
  unloading = true;
  static_cast<ObjectSystem*>(mSystems["ObjectSystem"])->ClearSystem();
}
System * Core::GetSystemByName(std::string systemName)
{
  return (mSystems.find(systemName) == mSystems.end()) ? nullptr : mSystems[systemName];
}

void Core::SaveLevel(std::string filename){
  JSONTranslator j;
  j.SaveLevelToFile(filename, GetSystem(ObjectSystem));
}


std::string Core::GetCurrentLevelName(){
  return currentLevel;
}