#include "DamageLocatorComponent.h"
#include "JSONTranslator.h"
#include "Object.h"
#include "MessagingSystem.h"
#include "BulletComponent.h"
#include "KnifeComponent.h"
#include "TransformComponent.h"
#include "PCControllerComponent.h"
#include "DamageArrowComponent.h"

DamageLocatorComponent::DamageLocatorComponent() : GameLogicComponent(GameLogicType::DAMAGELOCATOR)
{
  mName_ = "DamageLocatorComponent";
}
bool DamageLocatorComponent::Initialize()
{
  JSONTranslator j;
  for (int i = 0; i < NUMARROWS; ++i){
    arrows[i] = j.CreateObjectFromFile("Arrow.json");
    arrows[i]->save = false;
    arrows[i]->Register();
    arrows[i]->Initialize();
    arrows[i]->GetComponent(TransformComponent)->mScale(0.6f, 0.6, 1);
  }
  return true;
}
void DamageLocatorComponent::Update(double dt)
{
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      Object * me, *other;
      if (col->obj1 == mParent())
      {
        me = col->obj1;
        other = col->obj2;
        
      }
      else
      {
        me = col->obj2;
        other = col->obj1;
      }
      if ((other->GetComponent(BulletComponent) && me->GetComponent(PCControllerComponent))
        || (other->GetComponent(KnifeComponent) && !me->GetComponent(PCControllerComponent)))
      {
        vec2 v;
        v.x = other->GetComponent(TransformComponent)->mPositionX() - me->GetComponent(TransformComponent)->mPositionX();
        v.y = other->GetComponent(TransformComponent)->mPositionY() - me->GetComponent(TransformComponent)->mPositionY();
        v = normalize(v);
        v *= .75f;
        arrows[count]->GetComponent(TransformComponent)->mPosition(me->GetComponent(TransformComponent)->mPositionX() + v.x,
          me->GetComponent(TransformComponent)->mPositionY() + v.y, 3);
        float rad = atan2f(v.y, v.x);
        rad = degrees(rad);
        arrows[count]->GetComponent(TransformComponent)->mRotationZ(rad);
        arrows[count]->GetComponent(DamageArrowComponent)->Show(arrowVis);
        arrows[count]->GetComponent(DamageArrowComponent)->mOffsetx = v.x;
        arrows[count]->GetComponent(DamageArrowComponent)->mOffsety = v.y;
        count = (count + 1) % NUMARROWS;
        
      }
    }
  }
  for (int i = 0; i < NUMARROWS; ++i){
    vec2 v(arrows[i]->GetComponent(DamageArrowComponent)->mOffsetx, arrows[i]->GetComponent(DamageArrowComponent)->mOffsety);
    arrows[i]->GetComponent(TransformComponent)->mPosition(mParent()->GetComponent(TransformComponent)->mPositionX() + v.x,
      mParent()->GetComponent(TransformComponent)->mPositionY() + v.y, 3);
  }
}
void DamageLocatorComponent::Shutdown()
{
}
void DamageLocatorComponent::SetArrowVisibility(bool vis[5])
{
  for (int i = 0; i < 5; ++i){
    arrowVis[i] = vis[i];
  }
}
