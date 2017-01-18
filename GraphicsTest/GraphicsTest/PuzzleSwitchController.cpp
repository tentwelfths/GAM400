#include "PuzzleSwitchController.h"
#include "InputSystem.h"
#include "MessagingSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

int PuzzleSwitchController::GetControllerID()
{
  return controllerID_;
}

PuzzleSwitchController::PuzzleSwitchController() : GameLogicComponent(GameLogicType::PUZZLESWITCH), controllerID_(1), value_(0)
{
  AddMember(PuzzleSwitchController, controllerID_);
  mName_ = "PuzzleSwitchController";
}

bool PuzzleSwitchController::Initialize()
{
  //mParent_->mVisibility;
  return true;
}

void PuzzleSwitchController::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  
  int newVal = 0;
  if (input->isButtonPressed(controllerID_, 0))
  {
    newVal += 1;
  }
  if (input->isButtonPressed(controllerID_, 1))
  {
    newVal += 2;
  }
  if (input->isButtonPressed(controllerID_, 2))
  {
    newVal += 4;
  }
  if (value_ != newVal)
  {
    IMessage msg(MessageType::CHANGELEDS);
    ChangeLEDSMessage* msgData = reinterpret_cast<ChangeLEDSMessage*>(msg.data);
    msgData->controllerNum = controllerID_;
    for (int i = 0; i < 10; ++i)
    {
      if (i < value_+1)
      {
        msgData->state[i] = true;
      }
      else
      {
        msgData->state[i] = false;
      }
    }
    MessagingSystem* m = gCore->GetSystem(MessagingSystem);
    m->SendMessageToSystem(msg, "NetworkingSystem");
    value_ = newVal;
  }
  
}

void PuzzleSwitchController::Shutdown()
{

}