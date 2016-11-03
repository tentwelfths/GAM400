#include "PCCameraFollow.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "GraphicsSystem.h"

PCCameraFollow::PCCameraFollow() : GameLogicComponent(GameLogicType::CAMERA)
{
  mName_ = "PCCameraFollow";
}

bool PCCameraFollow::Initialize()
{
  return true;
}

void PCCameraFollow::Update(double dt)
{
  auto * graphicsCamera = &gCore->GetSystem(GraphicsSystem)->mMainCamera;
  graphicsCamera->x = mParent()->GetComponent(TransformComponent)->mPositionX();
  graphicsCamera->y = mParent()->GetComponent(TransformComponent)->mPositionY();
}
void PCCameraFollow::Shutdown()
{

}