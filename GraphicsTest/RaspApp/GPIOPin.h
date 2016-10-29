#pragma once
#include <string>

class GPIOPin
{
  public:
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
  std::string pinNum;
  std::string pinDir;
};