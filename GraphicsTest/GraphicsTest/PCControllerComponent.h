#include "PlayerControllerComponent.h"

class Object;

class PCControllerComponent : public PlayerControllerComponent
{
  public:
    PCControllerComponent();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void Movement(InputSystem* input);
    void Damage(int damage);
  private:
    int curCam;
    float sprintSpeed;
    float sprintTime;
    float spawnTime;
    bool isSprinting;
    Object * mHealthBar;
};