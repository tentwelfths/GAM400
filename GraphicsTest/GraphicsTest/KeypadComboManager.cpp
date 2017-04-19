//Fucking keep track of the combo and create and display the digits...
#include <Box2D\Box2D.h>
#include "KeypadComboManager.h"
#include "JSONTranslator.h"
#include "PuzzleHints.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "PuzzleObject.h"
#include "SpriteComponent.h"
#include "GraphicsSystem.h"
#include "MessagingSystem.h"

KeypadComboManager::KeypadComboManager() : GameLogicComponent(GameLogicType::COMBOMANAGER)
{
  mName_ = "KeypadComboManager";
  timerManager = nullptr;
  mCombination = "     ";
  mTimer = 0.0;
}

bool KeypadComboManager::Initialize()
{
  JSONTranslator j;
  auto * g = gCore->GetSystem(GraphicsSystem);
  
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
  mDigitFilenames[10] = "TimerNone.png";

  for (int i = 0; i < 5; ++i){
    mDigits[i] = j.CreateObjectFromFile("Empty.json");
    mDigits[i]->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[10]);
    mDigits[i]->GetComponent(SpriteComponent)->mTint({ 0, 1, 0 });
    glm::vec3 newTrans(mParent()->GetComponent(TransformComponent)->mPositionX() - ((5 / 2) - i), mParent()->GetComponent(TransformComponent)->mPositionY(), mParent()->GetComponent(TransformComponent)->mPositionZ() + 1);
    mDigits[i]->GetComponent(TransformComponent)->mPosition(newTrans);
    mDigits[i]->save = false;
    mDigits[i]->Initialize();
    mDigits[i]->Register();
  }

  return true;
}

void KeypadComboManager::Update(double dt)
{
  for (int i = 0; i < mParent()->mMessages_.size(); ++i){
    //Go through messages to get button press info
    if (mParent()->mMessages_[i].type == MessageType::KEYPADBUTTONPRESS){
      ButtonPress(reinterpret_cast<KeypadButtonPress*>(mParent()->mMessages_[i].data)->index);
    }
  }
  mTimer += dt;
  if (mTimer > 0.5){
    mTimer = 0;
    mDigits[mCurrentDigit]->mVisibility[4] = !mDigits[mCurrentDigit]->mVisibility[4];
  }

}

void KeypadComboManager::ButtonPress(char button){

  if (button >= '0' && button <= '9' && mCurrentDigit < 5){
    //new number
    mDigits[mCurrentDigit]->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[button - '0']);
    mCombination[mCurrentDigit] = button;
  }
  else if (button == 'b'){
    //backspace
    mDigits[mCurrentDigit]->mVisibility[4] = true;
    --mCurrentDigit;
    //mDigits[mCurrentDigit]->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[10]);
    //mCombination[mCurrentDigit] = ' ';
  }
  else if (button == 'f'){
    //backspace
    mDigits[mCurrentDigit]->mVisibility[4] = true;
    ++mCurrentDigit;
    //mDigits[mCurrentDigit]->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[10]);
    //mCombination[mCurrentDigit] = ' ';
  }
  else if (button == 'e'){
    //enter
    //TODO:
    //End Game Logic
    auto * o = gCore->GetSystem(ObjectSystem);
    auto * hintObj = o->GetFirstItemByName("HintHolder");
    auto * ph = hintObj->GetComponent(PuzzleHints);
    std::string theCode;
    for (int i = 0; i < 5; ++i)
    {
      theCode += std::to_string(ph->GetValue(i));
    }
    
    if (theCode == mCombination)
    {
      gCore->UnloadLevel();
      gCore->LoadLevel("Win.json");
    }
    else
    {
      //gCore->UnloadLevel();
      //gCore->LoadLevel("Lose.json");
      if (timerManager == nullptr){
        timerManager = gCore->GetSystem(ObjectSystem)->GetFirstItemByName("TimerManager");
      }
      IMessage msg(MessageType::SUBTRACTTIME);
      SubtractTime * st = reinterpret_cast<SubtractTime*>(msg.data);
      st->minutes = 1;
      st->seconds = 0;
      MessagingSystem* m = gCore->GetSystem(MessagingSystem);
      m->SendMessageToObject(msg, timerManager->ID);
    }
  }
}

void KeypadComboManager::Shutdown()
{

}