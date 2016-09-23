#pragma once
#include "System.h"
#include "Standard.h"

struct Times{
  std::string name;
  __int64 start;
  __int64 end;
  __int64 total;
};

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
  void SetSystemStart(std::string name);
  void SetSystemEnd();
  void PrintSystemTimeAnalysis();
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
  Times currentSystemTime;

  bool mStopped;
  std::vector<Times> times;
};