#include "PCCameraFollow.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "GraphicsSystem.h"
#include "ObjectSystem.h"
#include "InputSystem.h"

PCCameraFollow::PCCameraFollow() : GameLogicComponent(GameLogicType::CAMERA)
{
  mName_ = "PCCameraFollow";
}

bool PCCameraFollow::Initialize()
{
  mTargets.push_back("Monster");
  mTargets.push_back("ObjectiveZone1");
  mTargetIndex = 0;
  return true;
}

void PCCameraFollow::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  if (input->isKeyJustPressed(GLFW_KEY_Q)){
    mTargetIndex = (mTargetIndex - 1) % mTargets.size();
  }
  if (input->isKeyJustPressed(GLFW_KEY_E)){
    mTargetIndex = (mTargetIndex - 1) % mTargets.size();
  }
  auto * graphicsCamera = &gCore->GetSystem(GraphicsSystem)->mMainCamera;
  if (mTargets[mTargetIndex] == "Monster"){
    graphicsCamera->x = mParent()->GetComponent(TransformComponent)->mPositionX();
    graphicsCamera->y = mParent()->GetComponent(TransformComponent)->mPositionY();
  }
  else{
    Object * target = gCore->GetSystem(ObjectSystem)->GetFirstItemByName(mTargets[mTargetIndex]);
    graphicsCamera->x = target->mCamera.x;
    graphicsCamera->y = target->mCamera.y;
  }
}
void PCCameraFollow::Shutdown()
{

}