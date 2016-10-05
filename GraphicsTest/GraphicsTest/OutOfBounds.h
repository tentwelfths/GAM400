#include "GameLogicComponent.h"

class OutOfBounds : public GameLogicComponent
{
  public:
    OutOfBounds();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
  private:
    float deathHeight;
};