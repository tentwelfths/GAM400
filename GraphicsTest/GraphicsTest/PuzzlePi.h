#pragma once
#include "GameLogicComponent.h"
class InputSystem;
class CircleColliderComponent;

class PuzzlePi : public GameLogicComponent
{
public:
  PuzzlePi();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void UpdateTarget(CircleColliderComponent* circle, double dt);
  int Goal() { return nextUnlock_; }
private:
  float targetValueX_;
  float targetValueY_;
  float range_;
  float speed_;
  float delay_;
  float timeTillChange_;
  float boomTime_;
  int nextUnlock_;
  bool countDown_;
  bool start_;
};