#pragma once
#include "IComponent.h"

enum class GameLogicType { PLAYER, OUTOFBOUNDS, START, END, CAMERA, HOME, RANDOM, RESET, PUZZLEKNOB, PUZZLEKNOBNODE, PUZZLESWITCH, PUZZLESWITCHNODE, PROGRESS, PUZZLEPI, PUZZLEARROW, PUZZLESPAWN, PUZZLEOBJECT, PUZZLEHINTS };

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