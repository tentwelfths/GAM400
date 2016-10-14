#pragma once
#include "GameLogicComponent.h"
#include <glm/glm.hpp>

using namespace glm;
class Object;

class KnifeComponent : public GameLogicComponent
{
  public:
    KnifeComponent();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
  private:
    vec3 offset;
    Object* thePlayer;
    float attackTimer;
    float timeTillNextAttack;
    float dist;
};