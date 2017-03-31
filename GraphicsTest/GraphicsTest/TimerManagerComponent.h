#pragma once
#include "GameLogicComponent.h"

class TimerManagerComponent : public GameLogicComponent
{
public:
  TimerManagerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  Object* mMinute, *mTenSecond, *mSecond;
  std::string mDigitFilenames[10];
  double timer;
  int minutes, seconds;
};