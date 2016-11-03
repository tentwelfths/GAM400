#include "GameLogicComponent.h"


class StartManagerComponent : public GameLogicComponent
{
public:
  StartManagerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void SetBool(Object* thePlayer, bool state);
private:
  Object* mPCPlayer;
  Object* mConeMan;
  bool mP1Ready;
  bool mP2Ready;
  float mCountDown;
  float mTarget;
};