#pragma once
#include "System.h"
#include "Standard.h"


class PhysicsComponent;
class b2Body;
class b2BodyDef;
class b2World;
class MyListen;



class PhysicsSystem : public System
{
public:
  PhysicsSystem();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void RegisterComponent(PhysicsComponent * comp);
  b2Body* AddBody(b2BodyDef* theBody);
  MyListen* GetListen() { return theListen; }
private:
  std::vector<PhysicsComponent *> mComponents_;
  b2World* theWorld;
  MyListen* theListen;
  bool wasEditor;
};