#include "InputSystem.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "TransformComponent.h"
#include "Core.h"
#include "Object.h"
#include <cstring>

bool InputSystem::currInputs[NUMINPUTS] = { false };
bool InputSystem::prevInputs[NUMINPUTS] = { false };
bool Controller::currButt[NUMBUTT]      = { false };
bool Controller::prevButt[NUMBUTT]      = { false };
double InputSystem::xPos = 0;
double InputSystem::yPos = 0;

InputSystem::InputSystem()
{
  mName_ = "InputSystem";
}

bool InputSystem::Initialize()
{
  return true;
}

void InputSystem::Update(double dt)
{
  memcpy(prevInputs, currInputs, sizeof(bool) * NUMINPUTS);
  for (int i = 0; i < NUMCONT; ++i)
  {
    memcpy(theControllers[i].prevButt, theControllers[i].currButt, sizeof(bool) * NUMBUTT);
  }
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

void InputSystem::setButton(int contNum, int key, bool val)
{
  theControllers[contNum].currButt[key] = val;
}

void InputSystem::setMousePos(double xMouse, double yMouse)
{
  xPos = xMouse;
  yPos = yMouse;
  //std::cout << xMouse << ":" << yMouse << std::endl;
}

void InputSystem::setJoystick(int contNum, float xNewStick, float yNewStick)
{
  theControllers[contNum].theJoystick.xStick = xNewStick;
  theControllers[contNum].theJoystick.yStick = yNewStick;
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

bool InputSystem::isButtonPressed(int contNum, int key)
{
  return theControllers[contNum].currButt[key];
}

bool InputSystem::isButtonJustPressed(int contNum, int key)
{
  return theControllers[contNum].currButt[key] && !theControllers[contNum].prevButt[key];
}

bool InputSystem::isButtonUp(int contNum, int key)
{
  return !theControllers[contNum].currButt[key];
}

bool InputSystem::isButtonReleased(int contNum, int key)
{
  return !theControllers[contNum].currButt[key] && theControllers[contNum].prevButt[key];
}

Joystick InputSystem::getJoystick(int contNum)
{
  return theControllers[contNum].theJoystick;
}

void InputSystem::updateController(int contNum, std::vector<int> theButtons, std::vector<bool> theVal, float xNewStick, float yNewStick)
{
  int j = 0;
  for (auto & iter : theButtons)
  {
    setButton(contNum, iter, theVal[j]);
    ++j;
  }
  setJoystick(contNum, xNewStick, yNewStick);
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