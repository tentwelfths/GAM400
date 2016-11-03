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
private:
  Object* mPCPlayer;
  Object* mConeMan;
  bool mSpawn;
  float mCountDown;
  float mTarget;
};