#include "GameLogicComponent.h"

class DemoSpawner : public GameLogicComponent
{
  public:
    DemoSpawner();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
};