//Fucking keep track of the combo and create and display the digits...
#include <Box2D\Box2D.h>
#include "KeypadComboManager.h"
#include "JSONTranslator.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "PuzzleObject.h"
#include "SpriteComponent.h"
#include "GraphicsSystem.h"

KeypadComboManager::KeypadComboManager() : GameLogicComponent(GameLogicType::COMBOMANAGER)
{
  mName_ = "KeypadComboManager";
  mCombination = "     ";
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
}

void KeypadComboManager::ButtonPress(char button){

  if (button >= '0' && button <= '9' && mCurrentDigit < 5){
    //new number
    mDigits[mCurrentDigit]->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[button - '0']);
    mCombination[mCurrentDigit++] = button;
  }
  else if (button == 'b'){
    //backspace
    --mCurrentDigit;
    mDigits[mCurrentDigit]->GetComponent(SpriteComponent)->SetTexture(mDigitFilenames[10]);
    mCombination[mCurrentDigit] = ' ';
  }
  else if (button == 'e'){
    //enter
    //TODO:
    //End Game Logic
  }
}

void KeypadComboManager::Shutdown()
{

}