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
    void UpdateVis(Object* theTarget);
    float GetAngle(){ return mAngle; }
    float GetX() { return mDirX; }
    float GetY() { return mDirY; }
  private:
    float mDirX;
    float mDirY;
    float mAngle;
    float mSightRadius;
    Object* mPCPlayer;
    Object* mKnife;
    bool leds[10];
    int garbage;
    int clockwise;
    int counterclockwise;
};