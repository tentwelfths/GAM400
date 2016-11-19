#pragma once
#include <string>
#include <fstream>
#include <iostream>

class DebugClass{
public:
  DebugClass(std::string filename);
  void Log(std::string str);
  void Clear();
  void TurnOff();

private:
  std::string myFilename;
}