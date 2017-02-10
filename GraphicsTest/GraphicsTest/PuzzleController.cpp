#include <Box2D\Box2D.h>
#include "PuzzleController.h"
#include "InputSystem.h"
#include "ObjectSystem.h"
#include "CircleColliderComponent.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

PuzzleController::PuzzleController() : GameLogicComponent(GameLogicType::PLAYER), speed_(1.0f)
{
  AddMember(PuzzleController, speed_);
  mName_ = "PuzzleController";
}

bool PuzzleController::Initialize()
{
  return true;
}

void PuzzleController::Update(double dt)
{
  InputSystem* input = gCore->GetSystem(InputSystem);
  auto * rigid = mParent_->GetComponent(CircleColliderComponent);
  b2Vec2 newVel(0.0f, 0.0f);
  if (rigid)
  {
    Joystick dir = input->getJoystick(0);
    if (dir.x2Stick > 0.1 || dir.x2Stick < -0.1){
      newVel.x = dir.x2Stick * speed_;
    }
    if (dir.y2Stick > 0.1 || dir.y2Stick < -0.1){
      newVel.y = dir.y2Stick * speed_;
    }
    if (rigid->GetBody())
    {
      rigid->GetBody()->SetLinearVelocity(newVel);
    }
  }
}

void PuzzleController::Shutdown()
{

}