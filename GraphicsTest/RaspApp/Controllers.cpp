#include "Controllers.h"
#include <string>
#include <thread>
#include "GPIOPin.h"
#include "MCP3008SPI.h"

  std::string PINS[] = {"27","17","18","23","24","25","12","16","20","21"};

IController::IController(){

}

void IController::Initialize(){
  for(int i = 0; i < 10; ++i){
    mLeds[i].SetPinNum(PINS[i]);
    mLeds[i].ExportPin();
    mLeds[i].SetPinDir("out");
    mLeds[i].SetPinVal("1");
  }
  a2d = new mcp3008Spi("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
}

void IController::Uninitialize(){
  for(int i = 0; i < 10; ++i){
    mLeds[i].SetPinVal("0");
    mLeds[i].UnexportPin();
  }
  delete a2d;
}

std::string IController::GetJoystickData(){
  std::string inputstream;
  unsigned short x1 = a2d->GetChannelData(0);
  unsigned short y1 = a2d->GetChannelData(1);
  unsigned short x2 = a2d->GetChannelData(2);
  unsigned short y2 = a2d->GetChannelData(3);
  for(unsigned i = 0; i < sizeof(unsigned short); ++i){
    inputstream += static_cast<char *>(static_cast<void *>(&x1))[i];
  }
  for(unsigned i = 0; i < sizeof(unsigned short); ++i){
    inputstream += static_cast<char *>(static_cast<void *>(&y1))[i];
  }
  for(unsigned i = 0; i < sizeof(unsigned short); ++i){
    inputstream += static_cast<char *>(static_cast<void *>(&x2))[i];
  }
  for(unsigned i = 0; i < sizeof(unsigned short); ++i){
    inputstream += static_cast<char *>(static_cast<void *>(&y2))[i];
  }
  inputstream += (a2d->GetChannelData(4) > 15) ? '0' : '1';
  inputstream += (a2d->GetChannelData(5) > 15) ? '0' : '1';
  return inputstream;
}

ConeController::ConeController() : IController(){
  
}

void ConeController::Initialize(){
  IController::Initialize();
  threadInfo.bit1.SetPinNum("5");
  threadInfo.bit2.SetPinNum("6");
  threadInfo.bit1.ExportPin();
  threadInfo.bit1.SetPinDir("in");
  threadInfo.bit2.ExportPin();
  threadInfo.bit2.SetPinDir("in");
  std::thread t1(KnobTurned, &threadInfo);
}

void ConeController::Uninitialize(){
  IController::Uninitialize();
  threadInfo.bit1.UnexportPin();
  threadInfo.bit2.UnexportPin();
  t1.join();
}

std::string ConeController::GetInputData(){
  std::string inputstream = "";
  inputstream = GetJoystickData();
  inputstream += threadInfo.counter;
  threadInfo.counter = 0;
  return inputstream;
}

void * KnobTurned(ThreadInfo * t)
{
  while(!t->ctrl_c_pressed){
    //std::cout<<"CALLED"<<std::endl;
    std::string b1, b2;
    b1 = t->bit1.GetPinVal();
    b2 = t->bit2.GetPinVal();
    //std::cout<<b1<<"  "<<b2<<std::endl;
    int num = (b1 == "1") ? (1<<1) : (0<<1);
    num |= (b2 == "1") ? (1) : (0);
    if(num == t->prevState){
      //std::cout<<"No mov";
      continue;
    }
    switch(num){
      case 0b00:
      if(t->prevState == 0b01){// && prevprev == 0b11){
        --t->counter;
      }
      else if(t->prevState == 0b10){// && prevprev == 0b11){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
      case 0b01:
      if(t->prevState == 0b11){// && prevprev == 0b10){
        --t->counter;
      }
      else if(t->prevState == 0b00){// && prevprev == 0b10){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
      case 0b11:
      if(t->prevState == 0b10){// && prevprev == 0b00){
        --t->counter;
      }
      else if(t->prevState == 0b01){// && prevprev == 0b00){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
      case 0b10:
      if(t->prevState == 0b00){// && prevprev == 0b01){
        --t->counter;
      }
      else if(t->prevState == 0b11){// && prevprev == 0b01){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
    }
    t->prevState = num;
  }
  return nullptr;
}

GunController::GunController() : IController(){

}

void GunController::Initialize(){
  IController::Initialize();
  mRedSwitch.SetPinNum("13");
  mBlueSwitch.SetPinNum("26");
  mGreenSwitch.SetPinNum("19");
  mRedSwitch.ExportPin();
  mRedSwitch.SetPinDir("in");
  mBlueSwitch.ExportPin();
  mBlueSwitch.SetPinDir("in");
  mGreenSwitch.ExportPin();
  mGreenSwitch.SetPinDir("in");
}

void GunController::Uninitialize(){
  IController::Uninitialize();
  mRedSwitch.UnexportPin();
  mBlueSwitch.UnexportPin();
  mGreenSwitch.UnexportPin();
}

std::string GunController::GetInputData(){
  std::string inputstream = "";
  inputstream = GetJoystickData();
  inputstream += mRedSwitch.GetPinVal();
  inputstream += mBlueSwitch.GetPinVal();
  inputstream += mGreenSwitch.GetPinVal();
  return inputstream;
}