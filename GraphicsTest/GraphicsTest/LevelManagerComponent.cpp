#include "LevelManagerComponent.h"
#include "PCControllerComponent.h"
#include "ConeControllerComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"

LevelManagerComponent::LevelManagerComponent() : GameLogicComponent(GameLogicType::START), mSpawn(false), mCountDown(0.0f), mTarget(1.5f)
{
  mName_ = "LevelManagerComponent";
}

bool LevelManagerComponent::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  mPCPlayer = o->GetFirstItemByName("Player");
  mConeMan = o->GetFirstItemByName("Coneman");
  return true;
}

void LevelManagerComponent::Update(double dt)
{
  auto * thePCComponent = mPCPlayer->GetComponent(PCControllerComponent);
  auto * theConeComponent = mConeMan->GetComponent(ConeControllerComponent);
  if ( thePCComponent->GetHealth() <=0 )
  {
    SetBool(theConeComponent, false);
  }
  else if (theConeComponent->GetHealth() <= 0)
  {
    SetBool(thePCComponent, false);
  }
  if (mSpawn)
  {
    mCountDown += dt;
    if (mCountDown > mTarget)
    {
      gCore->LoadLevel("Start.json");
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