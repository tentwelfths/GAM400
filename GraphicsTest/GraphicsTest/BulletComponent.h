#pragma once
#include "GameLogicComponent.h"

class BulletComponent : public GameLogicComponent
{
public:
  BulletComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  bool contact;
  float lifeTime;
  float currTime;
  float contactDeathTime;
  float timeTillDead;
};