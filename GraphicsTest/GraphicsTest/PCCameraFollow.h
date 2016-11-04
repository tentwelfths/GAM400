#pragma once

#include "GameLogicComponent.h"

class Object;

class PCCameraFollow : public GameLogicComponent
{
public:
  PCCameraFollow();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
private:
  std::vector<std::string> mTargets;
  int mTargetIndex;
};