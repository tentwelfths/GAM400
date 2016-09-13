#include "System.h"
#include "Standard.h"

class PhysicsComponent;

enum class DynamicState { STATIC, DYNAMIC, Kinematic };

class PhysicsSystem : public System
{
public:
  PhysicsSystem();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void RegisterComponent(PhysicsComponent * comp);
private:
  std::vector<PhysicsComponent *> mComponents_;
};