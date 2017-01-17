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
  void AddObject(Object* obj, bool fromFile);
  Object * GetNthItemByName(std::string name, unsigned n);
  std::unordered_map<std::string, List<Object*> > mObjects;

  Object * CreateObjectFromFile(std::string filename);
  void CreateArchetypeFromObject(Object * obj);
  void RemoveDeadObject(unsigned int ID);
  void RemoveBornObject(unsigned int ID);
  void ClearSystem();

  unsigned int myCount;
  std::string frameData;
  unsigned int numObjects;
  std::string GetData(unsigned int ID, int num);
  std::string GetTransformData(unsigned int ID, int num);
  std::string Get2DPositionData(unsigned int ID);
  std::string GetTextureData(unsigned int ID);
  std::unordered_map<unsigned int, Object *> mObjectMap_;
  std::string GetCameraData(unsigned int ID);
  Object * CloneObject(Object * obj);
private:
  std::string GetData(Object * iter, int num);
  std::string GetTransformData(Object * iter, int num);
  std::string Get2DPositionData(Object * iter);
  std::string GetTextureData(Object * iter);
  std::string GetCameraData(Object * iter);

  std::unordered_map<unsigned int, std::pair<char, char>> deadObjects;
  std::unordered_map<unsigned int, std::pair<char, char>> bornObjects;
};