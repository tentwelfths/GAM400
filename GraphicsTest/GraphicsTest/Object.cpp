#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "IComponent.h"

Object::Object() : age(0), hasChanged(true)
{

}

bool Object::Initialize()
{
  age = 0;
  hasChanged = true;
  gCore->GetSystem(ObjectSystem)->AddObject(this);
  for (auto & iter : mComponents)
  {
    iter.second->Initialize();
  }
  return true;
}

void Object::Update(double dt)
{

}

void Object::Destroy(){
  dead = true;
}

void Object::Shutdown()
{

}

void Object::AddComponent(IComponent * comp)
{
  comp->mParent(this);
  mComponents.insert({ comp->mName(), comp });
}
