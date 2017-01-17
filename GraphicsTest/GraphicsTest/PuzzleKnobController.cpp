#include "PuzzleKnobController.h"
#include "InputSystem.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"

int PuzzleKnobController::GetControllerID()
{
  return controllerID_;
}

PuzzleKnobController::PuzzleKnobController() : GameLogicComponent(GameLogicType::PUZZLEKNOB), frequency(50)
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
    frequency += knobCon->knobDelta;
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