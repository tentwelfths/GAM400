#pragma once
#include "IComponent.h"

enum class GraphicsType {SPRITE};

class GraphicsComponent : public IComponent
{
public:
  GraphicsComponent(GraphicsType t);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  GraphicsType mType(){ return mType_; }
private:
  GraphicsType mType_;

};