#pragma once
#include "Standard.h"
class IComponent;
#include "Messages.h"

#define GetComponent(compName) GetComponentA<compName>(#compName)

class Object
{
public:
  Object();
  bool Initialize();

  void Update(double dt);

  void Destroy();

  void Shutdown();

  void AddComponent(IComponent * comp);

  template <typename T>
  T* GetComponentA(const char * compName);

  std::unordered_map<std::string, IComponent *> mComponents;
  void ReceiveMessage(IMessage msg){ mMessages_.push_back(msg); }

  std::string name;
  unsigned int ID;
  unsigned short age;
  bool hasChanged;
  std::string source;
  bool dead;
  std::vector<IMessage> mMessages_;
  bool mVisable;
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
