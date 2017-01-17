#include <Box2D\Box2D.h>
#include "PuzzleKnobNode.h"
#include "PuzzleProgess.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "SpriteComponent.h"
#include "PuzzleKnobController.h"
#include "ObjectSystem.h"

PuzzleKnobNode::PuzzleKnobNode() : GameLogicComponent(GameLogicType::PUZZLEKNOBNODE), targetFrequency_(0)
{
  AddMember(PuzzleKnobNode, targetFrequency_);
  mName_ = "PuzzleKnobNode";
}

bool PuzzleKnobNode::Initialize()
{
  return true;
}

void PuzzleKnobNode::Update(double dt)
{
  for (auto iter : mParent()->mMessages_)
  {
    if (iter.type == MessageType::COLLISIONSTARTED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      auto * o = gCore->GetSystem(ObjectSystem);
      auto * beat = o->GetFirstItemByName("Pulse")->GetComponent(PuzzleKnobController);
      auto * sprite = mParent()->GetComponent(SpriteComponent);
      float mod = beat->GetFrequency() * 0.1;
      if (targetFrequency_ == mod)
      {
        auto * progress = o->GetFirstItemByName("Tracker")->GetComponent(PuzzleProgress);
        progress->Success();
        sprite->mTint_.r = 0;
        sprite->mTint_.g = 1;
        sprite->mTint_.b = 0;
      }
      else if (targetFrequency_ - 1 == mod || targetFrequency_+1 == mod)
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

void PuzzleKnobNode::Shutdown()
{

}