#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "FramerateController.h"

FramerateController::FramerateController()
  : mTicksPerSecond(0), mDT(0.01667), mFramesPerSecond(60), mBaseTime(0),
  mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  mTicksPerSecond = (double)countsPerSec;
  mName_ = "FramerateController";
}

void FramerateController::Reset()
{
  mTicksPerSecond = 0;
  mDT = 0.01667;
  mFramesPerSecond = 60;
  mBaseTime = 0;

  mPausedTime = 0;
  mPrevTime = 0;
  mCurrTime = 0;
  mStopped = false;


  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  mTicksPerSecond = (double)countsPerSec;
}

void FramerateController::StartFrame()
{
  times.clear();
  QueryPerformanceCounter((LARGE_INTEGER*)&mFrameStart);
}

void FramerateController::EndFrame()
{
  QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);
  mDT = (mCurrTime - mFrameStart) / mTicksPerSecond;
  while (mDT < 1.0 / mFramesPerSecond)
  {
    QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);
    mDT = (mCurrTime - mFrameStart) / mTicksPerSecond;
  }
  //std::cout << mDT * 60.0 << std::endl;
}

void FramerateController::SetFramesPerSecond(unsigned framesPerSecond) { mFramesPerSecond = framesPerSecond; }
unsigned FramerateController::GetFramesPerSecond(){ return mFramesPerSecond; }
float FramerateController::GetDT()
{
  return (float)mDT;
}
float FramerateController::GetActualFramesPerSecond(){ return float(1.0 / mDT); }

void FramerateController::SetSystemStart(std::string name)
{
  currentSystemTime.name = name;
  QueryPerformanceCounter((LARGE_INTEGER*)&currentSystemTime.start);
}
void FramerateController::SetSystemEnd()
{
  QueryPerformanceCounter((LARGE_INTEGER*)&currentSystemTime.end);
  currentSystemTime.total = currentSystemTime.end - currentSystemTime.start;
  times.push_back(currentSystemTime);
}

void FramerateController::PrintSystemTimeAnalysis()
{
  __int64 totalFrameTime = 0;
  for (unsigned i = 0; i < times.size(); ++i){
    totalFrameTime += times[i].total;
  }
  for (unsigned i = 0; i < times.size(); ++i){
    std::cout << times[i].name << ": " << ((double)times[i].total / (double)totalFrameTime) * 100.0 << "%" << std::endl;
  }
}