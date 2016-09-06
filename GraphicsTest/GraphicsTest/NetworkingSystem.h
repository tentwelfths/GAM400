#pragma once

#include "System.h"

class Socket;

class NetworkingSystem : public System
{
public:
  NetworkingSystem();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  std::vector<Socket*> sockets;
};