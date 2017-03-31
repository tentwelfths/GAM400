#include <Box2D\Box2D.h>
#include "KeypadButtonComponent.h"
#include "JSONTranslator.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "ObjectSystem.h"
#include "SpriteComponent.h"
#include "GraphicsSystem.h"
#include "MessagingSystem.h"
#include "InputSystem.h"

KeypadButtonComponent::KeypadButtonComponent() : GameLogicComponent(GameLogicType::KEYPADBUTTON)
{
  AddMember(KeypadButtonComponent, mDigit);
  mName_ = "KeypadButtonComponent";
  manager = nullptr;
}

bool KeypadButtonComponent::Initialize()
{

  return true;
}

void KeypadButtonComponent::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  if (manager == nullptr){
    manager = gCore->GetSystem(ObjectSystem)->GetFirstItemByName("KeypadBackground");
  }
  
  if (input->isKeyJustPressed(GLFW_MOUSE_BUTTON_1)){
    auto * trans = mParent_->GetComponent(TransformComponent);
    double mX = input->GetMouseX();
    double mY = input->GetMouseY();
    if (mX >= trans->mPositionX() - trans->mScaleX() / 2.f && mX <= trans->mPositionX() + trans->mScaleX() / 2.f
      && mY >= trans->mPositionY() - trans->mScaleY() / 2.f && mY <= trans->mPositionY() + trans->mScaleY() / 2.f){
      IMessage msg(MessageType::KEYPADBUTTONPRESS);
      KeypadButtonPress * kpb = reinterpret_cast<KeypadButtonPress*>(msg.data);
      kpb->index = mDigit;
      MessagingSystem* m = gCore->GetSystem(MessagingSystem);
      m->SendMessageToObject(msg, manager->ID);
    }
  }
}

void KeypadButtonComponent::Shutdown()
{

}