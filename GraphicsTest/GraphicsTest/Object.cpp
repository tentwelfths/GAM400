#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "IComponent.h"

Object::Object() : mVisable(false), age(0), hasChanged(true)
{

}

bool Object::Initialize()
{
  age = 0;
  hasChanged = false;
  dead = false;
  for (auto & iter : mComponents)
  {
    iter.second->Initialize();
  }
  gCore->GetSystem(ObjectSystem)->AddObject(this);
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
