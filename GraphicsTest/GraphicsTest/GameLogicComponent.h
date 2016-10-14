#pragma once
#include "IComponent.h"

enum class GameLogicType { PLAYER, DEMOPLAYER, DEMODROPPER, OUTOFBOUNDS, RELOAD, KNIFE };

class GameLogicComponent : public IComponent
{
public:
  GameLogicComponent(GameLogicType t);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  GameLogicType mType(){ return mType_; }
private:
  GameLogicType mType_;

};