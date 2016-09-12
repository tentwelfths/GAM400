#pragma once
#include "Standard.h"
#include "System.h"
#include "List.h"
class Object;
class IComponent;

class ObjectSystem : public System
{
public:
  ObjectSystem();
  virtual void RegisterComponent(IComponent*);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  Object * GetFirstItemByName(std::string name);
  void AddObject(Object* obj);
  Object * GetNthItemByName(std::string name, unsigned n);
  std::unordered_map<std::string, List<Object*> > mObjects;
};