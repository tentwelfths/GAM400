
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
  virtual std::string GetInputData() = 0;
  std::string GetJoystickData();
  GPIOPin mLeds[10];
  mcp3008Spi * a2d;
};

struct ConeController : public IController
{
  ConeController();
  void Initialize();
  std::string GetInputData();
  void * KnobTurned(ThreadInfo* t);
  ThreadInfo threadInfo;
  std::thread t1;
};

struct GunController : public IController
{
  GunController();
  void Initialize();
  std::string GetInputData();
  GPIOPin mRedSwitch, mBlueSwitch, mGreenSwitch;
};