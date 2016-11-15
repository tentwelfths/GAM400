#include <Box2D\Box2D.h>
#include "ObjectiveOne.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "JSONTranslator.h"
#include "BoxColliderComponent.h"
#include "ObjectSystem.h"

ObjectiveOne::ObjectiveOne() : GameLogicComponent(GameLogicType::OBJECTIVEONE), zone(nullptr), zoneName("")
{
  AddMember(ObjectiveOne, zoneName);
  mName_ = "ObjectiveOne";
}

bool ObjectiveOne::Initialize()
{
  auto * o = gCore->GetSystem(ObjectSystem);
  zone = o->GetFirstItemByName(zoneName);
  if (!zone)
  {
    return true;
  }
  auto * bTrans = zone->GetComponent(TransformComponent);
  auto * bColl = zone->GetComponent(BoxColliderComponent);
  auto * trans = mParent()->GetComponent(TransformComponent);

  bTrans->mPosition(trans->mPosition());
  b2Vec2 bodyVec(bTrans->mPositionX(), bTrans->mPositionY());
  bColl->GetBody()->SetTransform(bodyVec, 0);

  return true;
}

void ObjectiveOne::Update(double dt)
{

}

void ObjectiveOne::Shutdown()
{

}