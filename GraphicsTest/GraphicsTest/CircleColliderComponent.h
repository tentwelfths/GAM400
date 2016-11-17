#pragma once
#include "PhysicsComponent.h"
//#include "PhysicsSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class b2Body;
struct b2BodyDef;
class b2World;
class b2PolygonShape;

class CircleColliderComponent : public PhysicsComponent
{
public:
  CircleColliderComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void ConfirmPosition();
  b2Body* GetBody() { return theBody; }
  b2BodyDef* GetType() { return type; }
  void SetRadius(float newRad) { mRadius = newRad; }
  void SetOffset(vec3 newOff) { mOffset = newOff; }
  void SetJustCollided(bool newState) { justCollided = newState; }
  void SetIsColliding(bool newState) { isColliding = newState; }
  float GetRadius() { return mRadius; }
  vec3 GetOffset() { return mOffset; }
  bool GetJustCollided() { return justCollided; }
  bool GetIsColliding() { return isColliding; }
private:
  float mRadius;
  vec3 mOffset;
  int dynamicState;
  float mDensity;
  float mFriction;
  bool mGhost;
  bool justCollided;
  bool isColliding;
  b2BodyDef* type;
  b2Body* theBody;
  b2CircleShape* circle;
};