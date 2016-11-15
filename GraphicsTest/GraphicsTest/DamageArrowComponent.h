#pragma once
#include "GameLogicComponent.h"


class DamageArrowComponent : public GameLogicComponent
{
public:
  DamageArrowComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void Show(bool vis[5]);
  float mOffsetx, mOffsety;
private:
  bool mVis;
  double mTimeAlive;

};