#pragma once
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
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
  void SetGhost(bool newGhost);
  void SwapGhost() { mGhost = !mGhost; }
  void SetRadius(float newRadius);
  void SetOffset(vec3 newOffset);
  bool GetGhost();
  float GetRadius();
  vec3 GetOffset();
private:
  int quadLoc;
  bool mGhost;
  float mRadius;
  vec3 mOffset;
};