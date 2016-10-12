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

  Object * CreateObjectFromFile(std::string filename);
  void CreateArchetypeFromObject(Object * obj);
  void RemoveDeadObject(unsigned int ID);
  void ClearSystem();

  unsigned int myCount;
  std::string frameData;
  unsigned int numObjects;
  std::string GetData(unsigned int ID);
  std::string GetTransformData(unsigned int ID);
  std::string GetTextureData(unsigned int ID);
private:
  std::string GetData(Object * iter);
  std::string GetTransformData(Object * iter);
  std::string GetTextureData(Object * iter);

  std::unordered_map<unsigned int, Object *> mObjectMap_;
  std::unordered_map<unsigned int, std::pair<char, char>> deadObjects;
  std::unordered_map<unsigned int, std::pair<char, char>> bornObjects;
};