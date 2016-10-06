#pragma once
#include "Standard.h"
#include "System.h"

#define GetSystem(sysName) GetSystemA<sysName>(#sysName)

class Core
{
public:
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void RegisterSystem(System * s);
  void UnloadLevel();
  void LoadLevel(std::string filename);
  template <typename T>
  T * GetSystemA(const char * systemName);
  bool editor;

private:
  bool unloading;
  std::string needToLoad;
  std::unordered_map<std::string, System*> mSystems;
  std::vector<System*> mSystemsOrdered;
};

template <typename T>
T * Core::GetSystemA(const char * systemName)
{
  std::string sysName = systemName;
  auto sys = mSystems.find(systemName);
  if (sys == mSystems.end())
  {
    return nullptr;
  }
  return reinterpret_cast<T*>(sys->second);
}
