#include <string>
#include <thread>
#include "GPIOPin.h"
#include "MCP3008SPI.h"

struct ThreadInfo{
  char counter = 0;
  GPIOPin bit1;
  GPIOPin bit2;
  int prevState = 0;
  bool ctrl_c_pressed;
};

struct IController
{
  IController();
  virtual void Initialize();
  virtual void Uninitialize();
  virtual std::string GetInputData() = 0;
  std::string GetJoystickData();
  GPIOPin mLeds[10];
  mcp3008Spi * a2d;
};

struct ConeController : public IController
{
  ConeController();
  void Initialize();
  virtual void Uninitialize();
  std::string GetInputData();
  
  ThreadInfo threadInfo;
  std::thread t1;
};

struct GunController : public IController
{
  GunController();
  void Initialize();
  virtual void Uninitialize();
  std::string GetInputData();
  GPIOPin mRedSwitch, mBlueSwitch, mGreenSwitch;
};

void * KnobTurned(ThreadInfo* t);