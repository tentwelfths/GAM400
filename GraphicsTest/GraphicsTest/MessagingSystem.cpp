#include "MessagingSystem.h"
#include "Globals.h"
#include "Core.h"
#include "ObjectSystem.h"
#include "Object.h"
#include "Messages.h"

MessagingSystem::MessagingSystem()
{

}
bool MessagingSystem::Initialize()
{ 
  return true; 
}
void MessagingSystem::Update(double dt)
{

}
void MessagingSystem::Shutdown()
{

}
void MessagingSystem::RegisterComponent(IComponent * comp)
{

}
void MessagingSystem::SendMessageToSystem(IMessage  msg, System * sys)
{
  sys->ReceiveMessage(msg);
}
void MessagingSystem::SendMessageToSystem(IMessage  msg, std::string systemName)
{
  gCore->GetSystemByName(systemName)->ReceiveMessage(msg);
}
void MessagingSystem::SendMessageToObject(IMessage msg, unsigned int ID)
{
  gCore->GetSystem(ObjectSystem)->mObjectMap_[ID]->ReceiveMessage(msg);
}