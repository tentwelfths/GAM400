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

void Core::RegisterSystem(System * s)
{
  mSystems.insert({ s->mName(), s });
  mSystemsOrdered.push_back(s);
}

bool Core::Initialize()
{
  editor = false;
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

  for (auto iter = mSystems.begin(); iter != mSystems.end(); ++iter)
    (*iter).second->Initialize();

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
  if (static_cast<ObjectSystem*>(mSystems["ObjectSystem"])->numObjects == 0){
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

  if (unloading)return;
  JSONTranslator j;
  j.LoadLevelFromFile(needToLoad);
  needToLoad = "";

}
void Core::UnloadLevel(){
  unloading = true;
  static_cast<ObjectSystem*>(mSystems["ObjectSystem"])->ClearSystem();
}