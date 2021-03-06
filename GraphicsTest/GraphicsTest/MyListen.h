#pragma once
#include "Box2D\Dynamics\b2WorldCallbacks.h"
#include "System.h"
#include "Standard.h"


class b2Contact;
class b2Fixture;
class CircleColliderComponent;

class MyListen : public b2ContactListener
{
public:
  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
private:
  CircleColliderComponent* objOne;
  CircleColliderComponent* objTwo;
};