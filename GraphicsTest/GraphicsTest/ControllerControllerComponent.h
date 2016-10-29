#include "PlayerControllerComponent.h"

class ControllerControllerComponent : public PlayerControllerComponent
{
public:
  ControllerControllerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void Movement(InputSystem* input);
  virtual void SpecialFunctionality(InputSystem* input);
private:
  int controllerID;
};