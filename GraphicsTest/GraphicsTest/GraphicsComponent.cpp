#include "GraphicsComponent.h"
#include "GraphicsSystem.h"
#include "Core.h"
#include "Globals.h"


GraphicsComponent::GraphicsComponent(GraphicsType t)
{
  GraphicsSystem * g = gCore->GetSystem(GraphicsSystem);
  g->RegisterComponent(this);

  mType_ = t;
  mName_ = "GraphicsComponent";
}

bool GraphicsComponent::Initialize()
{
  return true;
}

void GraphicsComponent::Update(double dt)
{

}

void GraphicsComponent::Shutdown()
{

}
