#include "GameLogicComponent.h"

class PlayerControllerComponent;

class LevelManagerComponent : public GameLogicComponent
{
public:
  LevelManagerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void SetBool(PlayerControllerComponent* thePlayer, bool state);
  bool mObjectives[2];
private:
  Object* mPCPlayer;
  Object* mConeMan;
  bool mSpawn;
  bool controlWins;
  float mCountDown;
  float mTarget;
};