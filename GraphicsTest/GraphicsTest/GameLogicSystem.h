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
    void SetPlayerActive(int controllerID, bool state);
    bool GetPlayerActive(int controllerID) { return aControllersPlaying[controllerID]; }
  private:
    std::vector<GameLogicComponent *> mComponents_;
    bool wasEditor;
    bool aControllersPlaying[4];
};