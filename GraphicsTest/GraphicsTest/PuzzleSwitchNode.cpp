#include <Box2D\Box2D.h>
#include "PuzzleSwitchNode.h"
#include "PuzzleProgess.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "PuzzleSwitchController.h"
#include "ObjectSystem.h"

PuzzleSwitchNode::PuzzleSwitchNode() : GameLogicComponent(GameLogicType::PUZZLESWITCHNODE), targetValue_(0)
{
  AddMember(PuzzleSwitchNode, targetValue_);
  mName_ = "PuzzleSwitchNode";
}

bool PuzzleSwitchNode::Initialize()
{
  return true;
}

void PuzzleSwitchNode::Update(double dt)
{
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      auto * o = gCore->GetSystem(ObjectSystem);
      auto * beat = o->GetFirstItemByName("Pulse")->GetComponent(PuzzleSwitchController);
      auto * sprite = mParent()->GetComponent(SpriteComponent);
      if (beat->GetValue() == targetValue_)
      {
        auto * progress = o->GetFirstItemByName("Tracker")->GetComponent(PuzzleProgress);
        progress->Success();
        sprite->mTint_.r = 0;
        sprite->mTint_.g = 1;
        sprite->mTint_.b = 0;
      }
      else if (beat->GetValue() == targetValue_ - 1 || beat->GetValue() == targetValue_ + 1)
      {
        sprite->mTint_.r = 0.5f;
        sprite->mTint_.g = 0.5f;
        sprite->mTint_.b = 0;
      }
      else
      {
        sprite->mTint_.r = 1;
        sprite->mTint_.g = 0;
        sprite->mTint_.b = 0;
      }
      sprite->hasChanged = true;
    }
    else if (iter.type == MessageType::COLLISIONENDED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      auto * sprite = mParent()->GetComponent(SpriteComponent);
      sprite->mTint_.r = 0;
      sprite->mTint_.g = 0;
      sprite->mTint_.b = 1;
      sprite->hasChanged = true;
    }
  }
}

void PuzzleSwitchNode::Shutdown()
{

}