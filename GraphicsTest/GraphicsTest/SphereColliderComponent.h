#pragma once
#include "PhysicsComponent.h"
//#include "PhysicsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class SphereColliderComponent : public PhysicsComponent
{
public:
  SphereColliderComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
};