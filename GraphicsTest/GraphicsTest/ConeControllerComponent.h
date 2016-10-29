#include "ControllerControllerComponent.h"

struct b2Vec2;

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
    b2Vec2 mDir;
    float mAngle;
    float mSightRadius;
    Object* mPCPlayer;
};