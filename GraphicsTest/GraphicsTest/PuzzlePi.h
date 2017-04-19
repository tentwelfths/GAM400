#pragma once
#include "GameLogicComponent.h"
class InputSystem;
class CircleColliderComponent;
class Object;

class PuzzlePi : public GameLogicComponent
{
public:
  PuzzlePi();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void UpdateTarget(double dt);
  void UpdateRange();
private:
  float targetValueX_;
  float targetValueY_;
  float range_;
  float speed_;
  float delay_;
  float countdown_;
  float killtime_;
  float timeTillChange_;
  bool dying_;
  Object * timerManager;
};