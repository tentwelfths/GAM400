#pragma once

#include <GL/glew.h>
#include <glfw3.h>

#include "System.h"
#include "Standard.h"

#define NUMINPUTS 350
#define NUMBUTT   5
#define NUMCONT   4
struct Joystick
{
  float xStick;
  float yStick;
};


struct Controller
{
  int numController;
  static bool currButt[NUMBUTT];
  static bool prevButt[NUMBUTT];
  Joystick theJoystick;
};

class InputSystem : public System
{
  public:
    InputSystem();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void setKey(int key, bool val);
    void setButton(int contNum, int key, bool val);
    void setJoystick(int contNum, float xNewStick, float yNewStick);
    void setRaspKey(int key, bool val, int clientNumber);
    void setMousePos(double xMouse, double yMouse);
    void updateController(int contNum, std::vector<int> theButtons, std::vector<bool> theVal, float xNewStick, float yNewStick);
    bool isKeyPressed(int key);
    bool isKeyJustPressed(int key);
    bool isKeyUp(int key);
    bool isKeyReleased(int key);
    bool isButtonPressed(int contNum, int key);
    bool isButtonJustPressed(int contNum, int key);
    bool isButtonUp(int contNum, int key);
    bool isButtonReleased(int contNum, int key);
    Joystick getJoystick(int contNum);
  private:
    static bool currInputs[NUMINPUTS];
    static bool prevInputs[NUMINPUTS];
    static double xPos;
    static double yPos;
    Controller theControllers[NUMCONT];
};
void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void inputMouseCallback(GLFWwindow *window, double xMouse, double yMouse);
void inputButtonCallback(GLFWwindow* window, int button, int action, int mods);