#pragma once
#include "GameLogicComponent.h"

class BulletComponent : public GameLogicComponent
{
public:
  BulletComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void SetPierce() { pierce = true; }
  void SetHoming() { homing = true; }
  void SetDamage(int newdamage) { damage = newdamage; }
  int GetDamage() { return damage; }
private:
  bool contact;
  bool pierce;
  bool homing;
  float lifeTime;
  float currTime;
  float contactDeathTime;
  float timeTillDead;
  int damage;
};