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
  bool posCheck = frequency >= 100 && knobCon->knobDelta == 1;
  bool negCheck = frequency <= 0 && knobCon->knobDelta == -1;
  if (posCheck || negCheck)
  {
    frequency += knobCon->knobDelta;
  }
  std::cout << frequency << std::endl;
}

void PuzzleKnobController::Shutdown()
{

}