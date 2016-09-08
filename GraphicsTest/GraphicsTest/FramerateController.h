#pragma once
#include "System.h"
#include "Standard.h"

class FramerateController : public System
{
public:
  FramerateController();

  bool Initialize(){ return true; }
  void Update(double dt){}
  void Shutdown(){}

  void Reset();
  void StartFrame();
  void EndFrame();
  void SetFramesPerSecond(unsigned framesPerSecond);
  unsigned GetFramesPerSecond();
  float GetDT();
  float GetActualFramesPerSecond();

private:
  double mTicksPerSecond;
  double mDT;
  unsigned mFramesPerSecond;
  __int64 mBaseTime;
  __int64 mPausedTime;
  __int64 mStopTime;
  __int64 mPrevTime;
  __int64 mCurrTime;
  __int64 mFrameStart;
  double mTotalTime;

  bool mStopped;
};