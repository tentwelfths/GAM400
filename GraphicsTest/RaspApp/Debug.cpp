#include "Debug.h"
#include <string>
#include <fstream>
#include <iostream>

DebugClass::DebugClass(std::string filename){
  myFilename = filename;
  on = true;
}
void DebugClass::Log(std::string str){
  if(!on)return;
  std::ofstream file;
  file.open(myFilename, std::fstream::app);
  if(file.is_open()){
    file << str << std::endl;
  }
  file.close();
}
void DebugClass::Clear(){
  if(!on)return;
  std::ofstream file;
  file.open(myFilename);
  file.close();
}

void DebugClass::TurnOff(){
  on = false;
}