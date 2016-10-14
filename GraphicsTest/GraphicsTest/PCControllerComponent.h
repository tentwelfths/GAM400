#include "PlayerControllerComponent.h"

class PCControllerComponent : public PlayerControllerComponent
{
  public:
    PCControllerComponent();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void Movement(InputSystem* input);
  private:
    int curCam;
    float sprintSpeed;
    bool isSprinting;
};