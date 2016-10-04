#pragma once

#include "IComponent.h"

class EditorComponent : public IComponent
{
public:
  EditorComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:

};