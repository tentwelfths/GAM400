#pragma once

#include "System.h"

class IComponent;
#include "Messages.h"

class MessagingSystem : public System
{
public:
  MessagingSystem();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void RegisterComponent(IComponent * comp);
  void SendMessageToSystem(IMessage  msg, std::string systemName);
  void SendMessageToSystem(IMessage  msg, System * sys);
  void SendMessageToObject(IMessage msg, unsigned int ID);
private:
  
};