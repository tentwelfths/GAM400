#include "Object.h"
#include "IComponent.h"


bool Object::Initialize()
{
  return true;
}

void Object::Update(double dt)
{

}

void Object::Shutdown()
{

}

void Object::AddComponent(IComponent * comp)
{
  comp->mParent(this);
  mComponents.insert({ comp->mName(), comp });
}
