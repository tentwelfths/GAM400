#include "NetworkingSystem.h"
#include "Globals.h"
#include "Socket.h"


NetworkingSystem::NetworkingSystem()
{
  mName_ = "GraphicsSystem";
}

bool NetworkingSystem::Initialize()
{
  //Init WSA
  //bind socket to listen for new peeps
}

void NetworkingSystem::Update(double dt)
{
  //grab some packets
  //grab any new devices that have connected
  //clean up any sockets that need to be closed and remove them from the vector... Vector may be a bad choice for data structure, will find out later.
}

void NetworkingSystem::Shutdown()
{
  //close all sockets
  //shutdown wsa
}
