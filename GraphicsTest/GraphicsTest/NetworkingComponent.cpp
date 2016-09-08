#include "NetworkingComponent.h"
#include "NetworkingSystem.h"
#include "Core.h"
#include "Globals.h"


NetworkingComponent::NetworkingComponent(NetworkingType t)
{
  NetworkingSystem * g = gCore->GetSystem(NetworkingSystem);
  g->RegisterComponent(this);

  mType_ = t;
  mName_ = "NetworkingComponent";
}

bool NetworkingComponent::Initialize()
{
  return true;
}

void NetworkingComponent::Update(double dt)
{

}

void NetworkingComponent::Shutdown()
{

}
