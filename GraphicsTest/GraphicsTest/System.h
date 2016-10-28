#pragma once
#include "Standard.h"

struct IMessage;

class System
{
public:
  virtual bool Initialize() = 0;
  virtual void Update(double dt) = 0;
  virtual void Shutdown() = 0;
  void ReceiveMessage(IMessage * msg){ mMessages_.push_back(msg); }
  std::string mName(){ return mName_; }
protected:
  std::string mName_;
  std::vector<IMessage*> mMessages_;
};