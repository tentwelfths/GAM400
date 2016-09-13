#pragma once

#include <GL/glew.h>
#include <glfw3.h>

#include "System.h"
#include "Standard.h"

#define NUMINPUTS 350

class InputSystem : public System
{
  public:
    InputSystem();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void setKey(int key, bool val);
    void setRaspKey(int key, bool val);
    void setMousePos(double xMouse, double yMouse);
    bool isKeyPressed(int key);
    bool isKeyJustPressed(int key);
    bool isKeyUp(int key);
    bool isKeyReleased(int key);
  private:
    static bool currInputs[NUMINPUTS];
    static bool prevInputs[NUMINPUTS];
    static double xPos;
    static double yPos;
};
void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void inputMouseCallback(GLFWwindow *window, double xMouse, double yMouse);
void inputButtonCallback(GLFWwindow* window, int button, int action, int mods);