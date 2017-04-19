#pragma once
#include "GameLogicComponent.h"

class KeypadComboManager : public GameLogicComponent
{
public:
  KeypadComboManager();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  void ButtonPress(char button);
  Object *(mDigits[5]), * timerManager;
  std::string mDigitFilenames[11];
  int mCurrentDigit;
  std::string mCombination;
};