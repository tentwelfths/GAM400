#include <Box2D\Box2D.h>
#include "TimerManagerComponent.h"
#include "JSONTranslator.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "PuzzleObject.h"
#include "SpriteComponent.h"
#include "GraphicsSystem.h"

TimerManagerComponent::TimerManagerComponent() : GameLogicComponent(GameLogicType::TIMERMANAGER)
{
  mName_ = "TimerManagerComponent";
}

bool TimerManagerComponent::Initialize()
{
  JSONTranslator j;
  auto * g = gCore->GetSystem(GraphicsSystem);

  minutes = 5;
  seconds = 0;

  mDigitFilenames[0] = "TimerZero.png";
  mDigitFilenames[1] = "TimerOne.png";
  mDigitFilenames[2] = "TimerTwo.png";
  mDigitFilenames[3] = "TimerThree.png";
  mDigitFilenames[4] = "TimerFour.png";
  mDigitFilenames[5] = "TimerFive.png";
  mDigitFilenames[6] = "TimerSix.png";
  mDigitFilenames[7] = "TimerSeven.png";
  mDigitFilenames[8] = "TimerEight.png";
  mDigitFilenames[9] = "TimerNine.png";

  timer = 0.0;

  mMinute = j.CreateObjectFromFile("Empty.json");
  mMinute->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[5]);
  mMinute->GetComponent(SpriteComponent)->mTint({ 1, 0, 0 });
  glm::vec3 newTrans(mParent()->GetComponent(TransformComponent)->mPositionX() - 1, mParent()->GetComponent(TransformComponent)->mPositionY(), mParent()->GetComponent(TransformComponent)->mPositionZ());
  mMinute->GetComponent(TransformComponent)->mPosition(newTrans);
  mMinute->save = false;
  mMinute->Initialize();
  mMinute->Register();

  mTenSecond = j.CreateObjectFromFile("Empty.json");
  mTenSecond->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[0]);
  mTenSecond->GetComponent(SpriteComponent)->mTint({ 1, 0, 0 });
  glm::vec3 newTrans2(mParent()->GetComponent(TransformComponent)->mPositionX() + 1, mParent()->GetComponent(TransformComponent)->mPositionY(), mParent()->GetComponent(TransformComponent)->mPositionZ());
  mTenSecond->GetComponent(TransformComponent)->mPosition(newTrans2);
  mTenSecond->save = false;
  mTenSecond->Initialize();
  mTenSecond->Register();

  mSecond = j.CreateObjectFromFile("Empty.json");
  mSecond->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[0]);
  mSecond->GetComponent(SpriteComponent)->mTint({ 1, 0, 0 });
  glm::vec3 newTrans3(mParent()->GetComponent(TransformComponent)->mPositionX() + 2, mParent()->GetComponent(TransformComponent)->mPositionY(), mParent()->GetComponent(TransformComponent)->mPositionZ());
  mSecond->GetComponent(TransformComponent)->mPosition(newTrans3);
  mSecond->save = false;
  mSecond->Initialize();
  mSecond->Register();

  return true;
}

void TimerManagerComponent::Update(double dt)
{
  timer += dt;
  if (timer > 1.0){
    timer -= 1.0;
    seconds -= 1;
    if (seconds < 0){
      seconds = 59;
      minutes -= 1;
      if (minutes < 0){
        //TODO: LOSE
        timer = -1000000;
      }
      
      mMinute->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[minutes]);
    }

    mTenSecond->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[seconds / 10]);
    mSecond->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[seconds % 10]);
  }
}

void TimerManagerComponent::Shutdown()
{
  
}