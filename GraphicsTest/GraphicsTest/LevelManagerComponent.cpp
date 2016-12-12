#include "LevelManagerComponent.h"
#include "PCControllerComponent.h"
#include "ConeControllerComponent.h"
#include "GunControllerComponent.h"
#include "JSONTranslator.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"

LevelManagerComponent::LevelManagerComponent() : GameLogicComponent(GameLogicType::START), mSpawn(false), mCountDown(0.0f), mTarget(1.5f)
{
  for (int i = 0; i < 2; ++i)
  {
    mObjectives[i] = false;
  }
  mName_ = "LevelManagerComponent";
}

bool LevelManagerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mPCPlayer = o->GetFirstItemByName("Player");
  mConeMan = o->GetFirstItemByName("Coneman");
  mGunMan = o->GetFirstItemByName("Gunman");
  return true;
}

void LevelManagerComponent::Update(double dt)
{
  for (int i = 0; i < 4; ++i)
  {
    if (mObjectives[i] == false)
    {
      controlWins = false;
      break;
    }
    controlWins = true;
  }
  auto * thePCComponent = mPCPlayer->GetComponent(PCControllerComponent);
  auto * theConeComponent = mConeMan->GetComponent(ConeControllerComponent);
  auto * theGunComponent = mGunMan->GetComponent(GunControllerComponent);
  if ( controlWins )
  {
    SetBool(theConeComponent, false);
  }
  else if (theConeComponent->GetCurrHealth() <= 0 && theGunComponent->GetCurrHealth() <= 0)
  {
    SetBool(thePCComponent, false);
  }
  if (mSpawn)
  {
    mCountDown += dt;
    if (mCountDown > mTarget)
    {
      gCore->UnloadLevel();
      if (controlWins)
      {
        gCore->LoadLevel("ControllersWinLevel.json");
      }
      else
      {
        gCore->LoadLevel("PCWinsLevel.json");
      }
    }
  }
}

void LevelManagerComponent::Shutdown()
{

}

void LevelManagerComponent::SetBool(PlayerControllerComponent* thePlayer, bool state)
{
  thePlayer->SetKillable(state);
  if (mSpawn == false)
  {
    mSpawn = true;
  }
}