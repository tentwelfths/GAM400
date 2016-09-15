#pragma once
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class BoxColliderComponent : public PhysicsComponent
{
public:
  BoxColliderComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void SetGhost(bool newGhost);
  void SwapGhost() { mGhost = !mGhost; }
  void SetHalfSize(vec3 newSize);
  void SetOffset(vec3 newOffset);
  bool GetGhost();
  vec3 GetHalfSize();
  vec3 GetOffset();
private:
  bool mGhost;
  vec3 mHalfSize;
  vec3 mOffset;
};