#pragma once
#include "PhysicsComponent.h"
//#include "PhysicsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class b2Body;
class b2BodyDef;
class b2World;
class b2PolygonShape;

class BoxColliderComponent : public PhysicsComponent
{
public:
  BoxColliderComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void ConfirmPosition();
  b2Body* GetBody() { return theBody; }
  b2BodyDef* GetType() { return type; }
  void SetHalfSize(vec3 newSize) { mHalfSize = newSize; }
  void SetOffset(vec3 newOff) { mOffset = newOff; }
  void SetJustCollided(bool newState) { justCollided = newState; }
  void SetIsColliding(bool newState) { isColliding = newState; }
  vec3 GetHalfSize() { return mHalfSize; }
  vec3 GetOffset() { return mOffset; }
  bool GetJustCollided() { return justCollided; }
  bool GetIsColliding() { return isColliding; }
private:
  vec3 mHalfSize;
  vec3 mOffset;
  int dynamicState;
  float mDensity;
  float mFriction;
  bool mGhost;
  bool justCollided;
  bool isColliding;
  b2BodyDef* type;
  b2Body* theBody;
  b2PolygonShape* box;
};