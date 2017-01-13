#include <Box2D\Box2D.h>
#include "PuzzleKnobNode.h"
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
      auto * beat = o->GetFirstItemByName("Coneman")->GetComponent(PuzzleKnobController);
      auto * sprite = mParent()->GetComponent(SpriteComponent);
      int mod = abs(targetFrequency_ - beat->GetFrequency());
      sprite->mTint_.r = mod;
      sprite->mTint_.g = 1-mod;
      sprite->mTint_.b = 0;
    }
    else if (iter.type == MessageType::COLLISIONENDED)
    {
      CollisionStartedMessage * col = reinterpret_cast<CollisionStartedMessage *>(iter.data);
      auto * sprite = mParent()->GetComponent(SpriteComponent);
      sprite->mTint_.r = 0;
      sprite->mTint_.g = 0;
      sprite->mTint_.b = 1;
    }
  }
}

void PuzzleKnobNode::Shutdown()
{

}