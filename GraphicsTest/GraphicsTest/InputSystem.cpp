#include "InputSystem.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "TransformComponent.h"
#include "GraphicsSystem.h"
#include "Core.h"
#include "Object.h"
#include <cstring>
#include "imgui.h"

bool InputSystem::currInputs[NUMINPUTS] = { false };
bool InputSystem::prevInputs[NUMINPUTS] = { false };
bool Controller::currButt[NUMBUTT]      = { false };
bool Controller::prevButt[NUMBUTT]      = { false };
float InputSystem::xPos = 0;
float InputSystem::yPos = 0;
double InputSystem::mouseDelta = 0;

static int controllerTracker = 0;

InputSystem::InputSystem()
{
  mName_ = "InputSystem";
}

Controller::Controller() : numController(controllerTracker), isPluggedIn(true)
{
  ++controllerTracker;
}

Joystick::Joystick() : x1Stick(0.0f), y1Stick(0.0f), x2Stick(0.0f), y2Stick(0.0f)
{
  
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
    theControllers[i].knobDelta = 0;
  }

  mouseDelta = 0;
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
  //std::cout << key << ":" << val << std::endl;
  currInputs[key] = val;
}

void InputSystem::setButton(int contNum, int key, bool val)
{
  theControllers[contNum].currButt[key] = val;
}

void InputSystem::setMousePos(float xMouse, float yMouse)
{
  xPos = xMouse;
  yPos = yMouse;
  //std::cout << xMouse << ":" << yMouse << std::endl;
}

void InputSystem::setJoystick(int contNum, float x1NewStick, float y1NewStick, float x2NewStick, float y2NewStick)
{
  theControllers[contNum].theJoystick.x1Stick = x1NewStick;
  theControllers[contNum].theJoystick.y1Stick = y1NewStick;
  theControllers[contNum].theJoystick.x2Stick = x2NewStick;
  theControllers[contNum].theJoystick.y2Stick = y2NewStick;
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

  //std::cout << std::endl << "CURR: " << theControllers[contNum].currButt[key] << std::endl;
  //std::cout << "PREV: " << theControllers[contNum].prevButt[key] << std::endl <<std::endl;
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

Controller InputSystem::getController(int contNum)
{
  return theControllers[contNum];
}

float InputSystem::GetMouseX(){ auto * gSys = gCore->GetSystem(GraphicsSystem);return xPos + gSys->mMainCamera.x; }
float InputSystem::GetMouseY(){ auto * gSys = gCore->GetSystem(GraphicsSystem);return yPos + gSys->mMainCamera.y; }

void InputSystem::updateController(int contNum, std::vector<int> theButtons, 
  std::vector<bool> theVal, float x1NewStick, float y1NewStick, float x2NewStick, 
  float y2NewStick)
{
  int j = 0;
  for (auto & iter : theButtons)
  {
    setButton(contNum, iter, theVal[j]);
    ++j;
  }
  setJoystick(contNum,  x1NewStick,  y1NewStick,  x2NewStick,  y2NewStick);
}
void InputSystem::updateController(int contNum, std::vector<int> theButtons, 
  std::vector<bool> theVal, float x1NewStick, float y1NewStick, float x2NewStick, 
  float y2NewStick, char knobDelta)
{
  int j = 0;
  for (auto & iter : theButtons)
  {
    setButton(contNum, iter, theVal[j]);
    ++j;
  }
  setJoystick(contNum, x1NewStick, y1NewStick, x2NewStick, y2NewStick);
  //if (knobDelta != 0)
  //  std::cout << (int)knobDelta << std::endl;
  if (knobDelta != 0)
  {
    int b = 10;
  }
  theControllers[contNum].knobDelta += knobDelta;
}

void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  InputSystem * i = gCore->GetSystem(InputSystem);
  ImGuiIO& io = ImGui::GetIO();
  
  switch (action)
  {
  case GLFW_RELEASE:
    io.KeysDown[key] = false;
    i->setKey(key, false);
    break;
  default:
    io.KeysDown[key] = true;
    if (io.WantCaptureKeyboard)
      return;
    i->setKey(key, true);
    break;
  }

}

void inputMouseCallback(GLFWwindow *window, double xMouse, double yMouse)
{
  auto * gSys = gCore->GetSystem(GraphicsSystem);
  InputSystem * i = gCore->GetSystem(InputSystem);
  GLint viewport[4]; //var to hold the viewport info
  GLdouble modelview[16]; //var to hold the modelview info
  GLdouble projection[16]; //var to hold the projection matrix info
  GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
  GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

  glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
  //glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
  glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

  winX = (float)xMouse;
  winY = (float)viewport[3] - (float)yMouse;
  glReadPixels((GLint)xMouse, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

  //get the world coordinates from the screen coordinates
  for (int i = 0; i < 16; ++i){
    projection[i] = gSys->Projection[i / 4][i%4];
    //modelview[i] = gSys->View[i / 4][i % 4];
  }
  gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
  //std::cout << worldX << ", " << worldY << std::endl;
  i->setMousePos((float)worldX, (float)worldY);
}

void inputButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  InputSystem * i = gCore->GetSystem(InputSystem);
  ImGuiIO& io = ImGui::GetIO();
  switch (action)
  {
  case GLFW_RELEASE:
    i->setKey(button, false);
    break;
  default:
    if (io.WantCaptureMouse)
      return;
    i->setKey(button, true);
    break;
  }
}

void inputScrollCallback(GLFWwindow* window, double x, double y)
{
  InputSystem * i = gCore->GetSystem(InputSystem);
  ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse)
  {
    io.MouseWheel = (float)y;
    return;
  }

  i->SetScrollDelta(y);
}

void inputCursorEnterCallback(GLFWwindow* window, int in)
{
  InputSystem * i = gCore->GetSystem(InputSystem);
  if (!in)
  {
    i->setMousePos(-1000, -1000);
  }
}