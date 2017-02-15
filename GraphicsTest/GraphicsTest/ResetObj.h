#pragma once
#include "GameLogicComponent.h"
class InputSystem;
class CircleColliderComponent;
class TransformComponent;

class ResetObj : public GameLogicComponent
{
  public:
    ResetObj();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void Reset(TransformComponent* trans, CircleColliderComponent* circle);
  private:
};