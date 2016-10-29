#pragma once
#include "GameLogicComponent.h"
class InputSystem;

class PlayerControllerComponent : public GameLogicComponent
{
public:
  PlayerControllerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  virtual void Movement(InputSystem* input);
  float GetSpeed() { return speed; }
  int GetHealth() { return health; }
  void Damage(int damage) { health -= damage; }
private:
  float speed;
  int playerID;
  int health;
};
