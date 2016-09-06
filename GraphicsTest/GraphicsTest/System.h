#pragma once
#include "Standard.h"

class System
{
public:
  virtual bool Initialize() = 0;
  virtual void Update(double dt) = 0;
  virtual void Shutdown() = 0;
  std::string mName(){ return mName_; }
protected:
  std::string mName_;
};