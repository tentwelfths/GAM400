#include "PuzzleKnobController.h"
#include "InputSystem.h"
#include "MessagingSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

int PuzzleKnobController::GetControllerID()
{
  return controllerID_;
}

PuzzleKnobController::PuzzleKnobController() : GameLogicComponent(GameLogicType::PUZZLEKNOB), frequency(50), preLeds(5)
{
  AddMember(PuzzleKnobController, controllerID_);
  mName_ = "PuzzleKnobController";
}

bool PuzzleKnobController::Initialize()
{
  return true;
}

void PuzzleKnobController::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  Controller* knobCon = &input->getController(GetControllerID());
  bool posCheck = frequency >= 100 && knobCon->knobDelta > 0;
  bool negCheck = frequency <= 0 && knobCon->knobDelta < 0;
  if (!posCheck && !negCheck)
  {
    frequency += knobCon->knobDelta * 4;
    int leds = frequency * 0.1;
    if (preLeds != leds)
    {
      IMessage msg(MessageType::CHANGELEDS);
      ChangeLEDSMessage* msgData = reinterpret_cast<ChangeLEDSMessage*>(msg.data);
      msgData->controllerNum = controllerID_;
      for (int i = 0; i < 10; ++i)
      {
        if (i < leds)
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
      preLeds = leds;
    }
  }
  if (frequency > 100)
  {
    frequency = 100;
  }
  else if (frequency < 0)
  {
    frequency = 0;
  }
  //std::cout << frequency << std::endl;
}

void PuzzleKnobController::Shutdown()
{

}