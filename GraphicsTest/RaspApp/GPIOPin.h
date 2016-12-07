#pragma once
#include <string>

class GPIOPin
{
  public:
  GPIOPin();
  void SetPinNum(std::string pinNum);
  GPIOPin(std::string pinNum);
  ~GPIOPin();
  void ExportPin();
  void UnexportPin();
  void SetPinDir(std::string dir);
  void SetPinVal(std::string val);
  void Update();
  std::string GetPinVal();

  private:
  std::string val;
  std::string prevVal;
  std::string pinNum_;
  std::string pinDir;
};