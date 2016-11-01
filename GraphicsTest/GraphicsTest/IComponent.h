#pragma once
#include "Standard.h"
#include "Metadata.h"
class Object;

class IComponent
{
public:
  virtual bool Initialize() = 0;
  virtual void Update(double dt) = 0;
  virtual void Shutdown() = 0;
  std::string mName(){ return mName_; }
  void mParent(Object * p){ mParent_ = p; }
  Object * mParent(){ return mParent_; }
  std::unordered_map<std::string, Mem *> members;
  bool clean;
  bool dead;
protected:
  Object * mParent_;
  std::string mName_;
};