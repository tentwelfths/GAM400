#include "System.h"
#include "Standard.h"

class GameLogicComponent;

class GameLogicSystem : public System
{
  public:
    GameLogicSystem();
    bool Initialize();
    void Update(double dt);
    void Shutdown();
    void RegisterComponent(GameLogicComponent * comp);
  private:
    std::vector<GameLogicComponent *> mComponents_;
    bool wasEditor;
};