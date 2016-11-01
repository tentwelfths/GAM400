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
    float GetAngle(){ return mAngle; }
    float GetX() { return mDirX; }
    float GetY() { return mDirY; }
  private:
    float mDirX;
    float mDirY;
    float mAngle;
    float mSightRadius;
    Object* mPCPlayer;
    bool leds[10];
    int garbage;
};