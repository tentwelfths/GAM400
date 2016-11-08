#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "IComponent.h"

Object::Object() : age(0), hasChanged(true)
{
  for (int i = 0; i < sizeof(mVisibility) /sizeof(mVisibility[0]); ++i){
    mVisibility[i] = true;
  }
}

void Object::Register()
{
  gCore->GetSystem(ObjectSystem)->AddObject(this);
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
  return true;
}

void Object::Update(double dt)
{
  mMessages_.clear();
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

void Object::AddComponent(std::string name)
{
  if (components.find(name) == components.end()) return;
  AddComponent(components[name]());
}

void Object::RemoveComponent(IComponent * comp)
{
  comp->dead = true;
  mComponents.erase(mComponents.find(comp->mName()));
  comp->Shutdown();
}