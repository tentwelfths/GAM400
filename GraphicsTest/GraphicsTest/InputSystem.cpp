#include "InputSystem.h"
#include "Globals.h"
#include "Core.h"
#include <cstring>

bool InputSystem::currInputs[NUMINPUTS] = { false };
bool InputSystem::prevInputs[NUMINPUTS] = { false };
float InputSystem::xPos = 0;
float InputSystem::yPos = 0;

InputSystem::InputSystem()
{
  mName_ = "";
}

bool InputSystem::Initialize()
{
  return true;
}

void InputSystem::Update(double dt)
{
  memcpy(prevInputs, currInputs, sizeof(bool) * NUMINPUTS);
}

void InputSystem::Shutdown()
{

}

void InputSystem::setKey(int key, bool val)
{
  currInputs[key] = val;
}

void InputSystem::setMousePos(double xMouse, double yMouse)
{
  xPos = xMouse;
  yPos = yMouse;
}

bool InputSystem::isKeyJustPressed(int key)
{
  return (currInputs[key] && !prevInputs[key]);
}

bool InputSystem::isKeyPressed(int key)
{
  return currInputs[key];
}

bool InputSystem::isKeyUp(int key)
{
  return !currInputs[key];
}

bool InputSystem::isKeyReleased(int key)
{
  return (!currInputs[key] && prevInputs[key]);
}

void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  InputSystem * i = gCore->GetSystem(InputSystem);
  switch (action)
  {
  case GLFW_RELEASE:
    i->setKey(key, false);
    break;
  default:
    i->setKey(key, true);
    break;
  }

}

void inputMouseCallback(GLFWwindow *window, double xMouse, double yMouse)
{
  InputSystem * i = gCore->GetSystem(InputSystem);
  i->setMousePos(xMouse, yMouse);
}

void inputButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  InputSystem * i = gCore->GetSystem(InputSystem);
  switch (action)
  {
  case GLFW_RELEASE:
    i->setKey(button, false);
    break;
  default:
    i->setKey(button, true);
    break;
  }
}