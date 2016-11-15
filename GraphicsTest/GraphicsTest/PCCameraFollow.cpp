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
  mTargets.push_back("ObjectiveZone2");
  mTargetIndex = 0;
  return true;
}

void PCCameraFollow::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  char dir = 0;

  Object * target = nullptr;

  if (input->isKeyJustPressed(GLFW_KEY_Q)){
    dir = -1;
  }
  if (input->isKeyJustPressed(GLFW_KEY_E)){
    dir = 1;
  }
  auto * graphicsCamera = &gCore->GetSystem(GraphicsSystem)->mMainCamera;
  while (true){
    mTargetIndex += dir;
    if (mTargetIndex < 0) mTargetIndex += mTargets.size();
    mTargetIndex = mTargetIndex % mTargets.size();

    if (mTargets[mTargetIndex] == "Monster"){
      graphicsCamera->x = mParent()->GetComponent(TransformComponent)->mPositionX();
      graphicsCamera->y = mParent()->GetComponent(TransformComponent)->mPositionY();
      break;
    }
    else{
      target = gCore->GetSystem(ObjectSystem)->GetFirstItemByName(mTargets[mTargetIndex]);
      if (target){
        graphicsCamera->x = target->GetComponent(TransformComponent)->mPositionX();
        graphicsCamera->y = target->GetComponent(TransformComponent)->mPositionY();
        break;
      }
    }
  }
}
void PCCameraFollow::Shutdown()
{

}