#include "ChangeSpriteComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "PuzzleRightStick.h"
#include "PuzzlePi.h"
#include "SpriteComponent.h"

ChangeSpriteComponent::ChangeSpriteComponent() : GameLogicComponent(GameLogicType::CHANGE)
{
  AddMember(ChangeSpriteComponent, changeTo);
  mName_ = "ChangeSpriteComponent";
}

bool ChangeSpriteComponent::Initialize()
{
  return true;
}

void ChangeSpriteComponent::Update(double dt)
{
  auto* o = gCore->GetSystem(ObjectSystem);
  auto* objA = o->GetFirstItemByName("Arrow");
  auto* arrow = objA->GetComponent(PuzzleRightStick);
  auto* objT = o->GetFirstItemByName("TempControl");
  auto* goal = objT->GetComponent(PuzzlePi);
  if (arrow->Completed() > goal->Goal())
  {
    auto* sprite = mParent()->GetComponent(SpriteComponent);
    if (sprite->mTextureName != changeTo)
    {
      sprite->SetTexture(changeTo);
    }
    
    
  }
}

void ChangeSpriteComponent::Shutdown()
{

}