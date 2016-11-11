#include "PlayerControllerComponent.h"

class ControllerControllerComponent : public PlayerControllerComponent
{
public:
  ControllerControllerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void Movement(InputSystem* input);
  virtual void Shoot(InputSystem* input, double dt);
  virtual void SpecialFunctionality(InputSystem* input);
  virtual void Reload(InputSystem* input);
  float GetBulletSpeed() { return bulletSpeed; }
  int GetControllerID() { return controllerID; }
protected:
  int currAmmo;
  int maxAmmo;
  float bulletSpeed;
  float shotTimer;
  float shotCD;
  int controllerID;
};