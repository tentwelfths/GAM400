#include "ControllerControllerComponent.h"

class Object;

class GunControllerComponent : public ControllerControllerComponent
{
public:
  GunControllerComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void Shoot(InputSystem* input, double dt);
  void SpecialFunctionality(InputSystem* input);
  Object* CreateBullet(float x, float y);
private:
  bool mSpreadOrRapid;
  bool mBigOrPierce;
  bool mHomingOrDamage;
};