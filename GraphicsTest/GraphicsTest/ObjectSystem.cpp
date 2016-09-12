#include "Object.h"
#include "ObjectSystem.h"

ObjectSystem::ObjectSystem()
{
  mName_ = "ObjectSystem";
}

void     ObjectSystem::RegisterComponent(IComponent*)
{

}

bool     ObjectSystem::Initialize()
{
  return true;
}

void     ObjectSystem::Update(double dt)
{

}

void     ObjectSystem::Shutdown()
{

}

void ObjectSystem::AddObject(Object * obj)
{
  mObjects[obj->name].AddToFront(obj);
}

Object * ObjectSystem::GetFirstItemByName(std::string name)
{
  return mObjects[name].head->value;
}
Object * ObjectSystem::GetNthItemByName(std::string name, unsigned n)
{
  int i = 0;
  auto head = mObjects[name].head;
  while (i < n && head)
  {
    ++i;
    head = head->next;
  }
  if (!head)return nullptr;
  return head->value;
}