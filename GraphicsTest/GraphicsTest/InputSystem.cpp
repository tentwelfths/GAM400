#include "InputSystem.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "TransformComponent.h"
#include "Core.h"
#include "Object.h"
#include <cstring>

bool InputSystem::currInputs[NUMINPUTS] = { false };
bool InputSystem::prevInputs[NUMINPUTS] = { false };
double InputSystem::xPos = 0;
double InputSystem::yPos = 0;

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

void InputSystem::setRaspKey(int key, bool val, int clientNumber)
{
  Object * obj = nullptr;
  int realKey = 0;
  switch (key) //can add more as needed.
  {
  case 16:
    realKey = GLFW_KEY_Q;
    break;
  case 17:
    realKey = GLFW_KEY_W;
    break;
  case 30:
    realKey = GLFW_KEY_A;
    break;
  case 31:
    realKey = GLFW_KEY_S;
    break;
  case 32:
    realKey = GLFW_KEY_D;
    break;
  case 57:
    realKey = GLFW_KEY_SPACE;
    break;
  }
  setKey(realKey, val);
}

void InputSystem::setKey(int key, bool val)
{
  std::cout << key << ":" << val << std::endl;
  currInputs[key] = val;
}

void InputSystem::setMousePos(double xMouse, double yMouse)
{
  xPos = xMouse;
  yPos = yMouse;
  //std::cout << xMouse << ":" << yMouse << std::endl;
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