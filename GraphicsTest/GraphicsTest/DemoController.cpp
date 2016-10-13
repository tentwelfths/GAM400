#include <Box2D\Box2D.h>
#include "DemoController.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "System.h"
#include "IComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"
#include "Standard.h"
#include "System.h"
#include "GraphicsSystem.h"

DemoController::DemoController() :GameLogicComponent(GameLogicType::DEMOPLAYER), speed(1.1f)
{
  AddMember(DemoController, speed);
  mName_ = "DemoController";
}

bool DemoController::Initialize()
{
  return true;
}

void DemoController::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  auto * graphics = gCore->GetSystem(GraphicsSystem);
  auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  b2Vec2 newVel(0.0f, 0.0f);
  if (rigid)
  {
    Joystick j = input->getJoystick(0);
    if (j.xStick > 0.1 || j.xStick < -0.1)
    {
      newVel.x = j.xStick * speed;
    }
    if (rigid->GetBody())
    {
      rigid->GetBody()->SetLinearVelocity(newVel);
    }
  }
  if (rigid->GetJustCollided())
  {
    rigid->SetJustCollided(false);
    std::cout << "HIT" << std::endl;
    mParent()->Destroy();
  }
  auto * trans = mParent_->GetComponent(TransformComponent);
  if (trans->mPositionX() > 10.0f)
  {
    trans->mPositionX(-9.9f);
    rigid->GetBody()->SetTransform(b2Vec2(trans->mPositionX(), trans->mPositionY()),trans->mRotationZ());
  }
  else if (trans->mPositionX() < -10.0f)
  {
    trans->mPositionX(9.9f);
    rigid->GetBody()->SetTransform(b2Vec2(trans->mPositionX(), trans->mPositionY()), trans->mRotationZ());
  }
  if (input->isKeyPressed(GLFW_KEY_LEFT)){
    graphics->mMainCamera.x -= 0.1;
  }
  if (input->isKeyPressed(GLFW_KEY_RIGHT)){
    graphics->mMainCamera.x += 0.1;
  }
  if (input->isKeyPressed(GLFW_KEY_DOWN)){
    graphics->mMainCamera.y -= 0.1;
  }
  if (input->isKeyPressed(GLFW_KEY_UP)){
    graphics->mMainCamera.y += 0.1;
  }
  if (input->isKeyPressed(GLFW_KEY_PAGE_DOWN)){
    graphics->mMainCamera.zoom -= 0.1;
  }
  if (input->isKeyPressed(GLFW_KEY_PAGE_UP)){
    graphics->mMainCamera.zoom += 0.1;
  }
}

void DemoController::Shutdown()
{

}