#include "Debug.h"

DebugClass::DebugClass(std::string filename){
  myFilename = filename;
  on = true;
}
void DebugClass::Log(std::string str){
  if(!on)return;
  std::ifstream file;
  file.open(myFilename, std::fstream::app);
  if(file.is_open()){
    file << str << std::endl;Class
  }
  file.close();
}
void DebugClass::Clear(){
  if(!on)return;
  std::ifstream file;
  file.open(myFilename);
  file.close();
}

void DebugClass::TurnOff(){
  on = false;
}