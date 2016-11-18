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
  Joystick();
  float x1Stick;
  float y1Stick;
  float x2Stick;
  float y2Stick;
};


struct Controller
{
  Controller();
  int numController;
  bool isPluggedIn;
  static bool currButt[NUMBUTT];
  static bool prevButt[NUMBUTT];
  Joystick theJoystick;
  char knobDelta;
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
    void setJoystick(int contNum, float x1NewStick, float y1NewStick, float x2NewStick, float y2NewStick);
    void setRaspKey(int key, bool val, int clientNumber);
    void setMousePos(float xMouse, float yMouse);
    void updateController(int contNum, std::vector<int> theButtons, std::vector<bool> theVal, float x1NewStick, float y1NewStick, float x2NewStick, float y2NewStick);
    void updateController(int contNum, std::vector<int> theButtons, std::vector<bool> theVal, float x1NewStick, float y1NewStick, float x2NewStick, float y2NewStick, char knobDelta);
    bool isKeyPressed(int key);
    bool isKeyJustPressed(int key);
    bool isKeyUp(int key);
    bool isKeyReleased(int key);
    bool isButtonPressed(int contNum, int key);
    bool isButtonJustPressed(int contNum, int key);
    bool isButtonUp(int contNum, int key);
    bool isButtonReleased(int contNum, int key);
    Joystick getJoystick(int contNum);
    Controller getController(int contNum);
    float GetMouseX();
    float GetMouseY();
    void SetScrollDelta(double delta){ mouseDelta = delta; }
    double GetScrollDelta(){ return mouseDelta; }
  private:
    static bool currInputs[NUMINPUTS];
    static bool prevInputs[NUMINPUTS];
    static float xPos;
    static float yPos;
    static double mouseDelta;
    Controller theControllers[NUMCONT];
};
void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void inputMouseCallback(GLFWwindow *window, double xMouse, double yMouse);
void inputButtonCallback(GLFWwindow* window, int button, int action, int mods);
void inputScrollCallback(GLFWwindow* window, double x, double y);