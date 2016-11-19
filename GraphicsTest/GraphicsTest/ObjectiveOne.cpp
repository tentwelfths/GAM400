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
  JSONTranslator j;
  Object * b;
  zone = j.CreateObjectFromFile("ObjectZone.json");
  if (!zone)
  {
    return true;
  }
  auto * bTrans = zone->GetComponent(TransformComponent);
  auto * bColl = zone->GetComponent(BoxColliderComponent);
  auto * trans = mParent()->GetComponent(TransformComponent);

  bTrans->mPosition(trans->mPositionX(), trans->mPositionY(), trans->mPositionZ()-1.0f);
  //b2Vec2 bodyVec(bTrans->mPositionX(), bTrans->mPositionY());
  //bColl->GetBody()->SetTransform(bodyVec, 0);
  zone->name = zoneName;
  zone->Initialize();
  zone->Register();
  return true;
}

void ObjectiveOne::Update(double dt)
{

}

void ObjectiveOne::Shutdown()
{

}