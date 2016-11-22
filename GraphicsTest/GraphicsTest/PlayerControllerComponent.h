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
  int GetMaxHealth() { return maxHealth; }
  int GetCurrHealth() { return currHealth; }
  bool GetKillable() { return killable; }
  virtual void Damage(int damage) { currHealth -= damage; }
  void Kill() { alive = false; }
  void Heal() { currHealth = maxHealth; }
  void Miracle() { alive = true; }
  void SetKillable(bool state) { killable = state; }
protected:
  bool alive;
  float speed;
  int playerID;
  int maxHealth;
  int currHealth;
  bool killable;
};
