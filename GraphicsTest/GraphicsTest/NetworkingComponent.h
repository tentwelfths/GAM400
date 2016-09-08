#pragma once
#include "IComponent.h"

enum class NetworkingType { SPRITE };

class NetworkingComponent : public IComponent
{
public:
  NetworkingComponent(NetworkingType t);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  virtual void SendData(SOCKET &s);
  NetworkingType mType(){ return mType_; }
private:
  NetworkingType mType_;
};