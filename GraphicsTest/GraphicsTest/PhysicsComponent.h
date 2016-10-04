#pragma once
#include "IComponent.h"

enum class PhysicsType { RIGIDBODY, BOXCOLLIDER, SPHERECOLLIDER, PHYSICSMATERIAL };

class PhysicsComponent : public IComponent
{
public:
  PhysicsComponent(PhysicsType t);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  virtual void ConfirmPosition();
  PhysicsType mType(){ return mType_; }
private:
  PhysicsType mType_;

};