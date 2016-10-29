#include "ControllerControllerComponent.h"

class ConeControllerComponent : public ControllerControllerComponent
{
  public:
    ConeControllerComponent();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void SpecialFunctionality(InputSystem* input);
    void UpdateCone();
  private:
    float mDirX;
    float mDirY;
    float mAngle;
    float mSightRadius;
    Object* mPCPlayer;
};