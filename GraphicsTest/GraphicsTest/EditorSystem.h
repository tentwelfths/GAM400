#pragma once
#include "System.h"

class EditorComponent;
class Object;

class EditorSystem : public System
{
public:
  EditorSystem();
  virtual void RegisterComponent(EditorComponent*);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  void Select(Object * obj);
  void DisplayImGUI();
private:
  Object * selected;
  std::vector<EditorComponent *> mComponents_;
};