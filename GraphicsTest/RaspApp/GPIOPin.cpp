#include "GPIOPin.h"
#include <iostream>
#include <fstream>
#include <string>

GPIOPin::GPIOPin(std::string pinNum) : pinNum_(pinNum){
}

GPIOPin::~GPIOPin(){
  UnexportPin();
}

void GPIOPin::ExportPin(){
    std::string pinPath = "/sys/class/gpio/export";
    std::ofstream pinFile(pinPath.c_str()); 
    if (pinFile.is_open() == false){
        std::cout << "Failed to export pin: #"<< pinNum_ <<" ."<< std::endl;
        return;
    }

    pinFile << pinNum_; 
    pinFile.close();
}

void GPIOPin::UnexportPin(){
    std::string pinPath = "/sys/class/gpio/unexport";
    std::ofstream pinFile(pinPath.c_str()); 
    if (pinFile.is_open() == false){
        std::cout << "Failed to export pin: #"<< pinNum_ <<" ."<< std::endl;
        return;
    }

    pinFile << pinNum_; 
    pinFile.close();
}

void GPIOPin::SetPinDir(std::string dir){
  std::string pinDirPath ="/sys/class/gpio/gpio" + pinNum_ + "/direction";
  pinDir = dir;
  std::ofstream pinDirFile(pinDirPath.c_str());
  if (pinDirFile.is_open() == false){
      std::cout << "Failed to set direction of pin: #"<< pinNum_ <<" ."<< std::endl;
      return;
  }

  pinDirFile << dir;
  pinDirFile.close();
}

void GPIOPin::SetPinVal(std::string val){
  std::string pinValPath ="/sys/class/gpio/gpio" + pinNum_ + "/value";
  std::ofstream pinValFile(pinValPath.c_str());
  if (pinValFile.is_open() == false){
      std::cout << "Failed to set value of pin: #"<< pinNum_ <<" ."<< std::endl;
      return;
  }

  pinValFile << val;
  pinValFile.close();
}

void GPIOPin::Update(){
  if(pinDir == "in")
  {
    prevVal = val;
    val = GetPinVal();
  }
}

std::string GPIOPin::GetPinVal(){
  std::string pinValPath ="/sys/class/gpio/gpio" + pinNum_ + "/value";
  std::ifstream pinValFile(pinValPath.c_str());
  if (pinValFile.is_open() == false){
      std::cout << "Failed to set value of pin: #"<< pinNum_ <<" ."<< std::endl;
      return "0";
  }
  std::string val;
  pinValFile >> val;
  val = (val == "0") ? "0" : "1";
  pinValFile.close();
  return val;
}