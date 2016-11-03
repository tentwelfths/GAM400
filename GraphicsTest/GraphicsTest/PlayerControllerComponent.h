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
  bool GetAlive() { return alive; }
  float GetSpeed() { return speed; }
  int GetHealth() { return health; }
  bool GetKillable() { return killable; }
  void Damage(int damage) { health -= damage; }
  void Kill() { alive = false; }
  void Miracle() { alive = true; }
  void SetKillable(bool state) { killable = state; }
private:
  bool alive;
  float speed;
  int playerID;
  int health;
  bool killable;
};
