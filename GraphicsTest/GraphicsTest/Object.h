#pragma once
#include "Standard.h"
class IComponent;

#define GetComponent(compName) GetComponentA<compName>(#compName)

class Object
{
public:
  bool Initialize();

  void Update(double dt);

  void Shutdown();

  void AddComponent(IComponent * comp);

  template <typename T>
  T* GetComponentA(const char * compName);

private:
  std::unordered_map<std::string, IComponent *> mComponents;

};

template <typename T>
T* Object::GetComponentA(const char * componentName)
{
  std::string compName = componentName;
  auto component = mComponents.find(compName);
  if (component == mComponents.end())
  {
    return nullptr;
  }
  return reinterpret_cast<T*>(component->second);
}
