#include "ControllerCameraFollow.h"
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include "GraphicsSystem.h"
#include "ConeControllerComponent.h"
#include "MessagingSystem.h"

ControllerCameraFollow::ControllerCameraFollow() : GameLogicComponent(GameLogicType::CAMERA)
{
  mName_ = "ControllerCameraFollow";
}

bool ControllerCameraFollow::Initialize()
{
  return true;
}

void ControllerCameraFollow::Update(double dt)
{
  //auto * graphicsCamera = &gCore->GetSystem(GraphicsSystem)->mMainCamera;
  auto * trans = mParent()->GetComponent(TransformComponent);
  float x = mParent()->mCamera.x;
  float y = mParent()->mCamera.y;
  bool xCheck = trans->mPositionX() > x - EPIFORTRANS && trans->mPositionX() < x + EPIFORTRANS;
  bool yCheck = trans->mPositionY() > y - EPIFORTRANS && trans->mPositionY() < y + EPIFORTRANS;

  if (!xCheck || !yCheck)
  {
    mParent()->mCamera.x = mParent()->GetComponent(TransformComponent)->mPositionX();
    mParent()->mCamera.y = mParent()->GetComponent(TransformComponent)->mPositionY();
    IMessage msg(MessageType::CAMERAMOVE);
    CameraMoveMessage* msgData = reinterpret_cast<CameraMoveMessage*>(msg.data);
    ControllerControllerComponent * controller = mParent()->GetComponent(ConeControllerComponent);
    if (controller == nullptr){
      //try a different controller
    }
    msgData->controllerNum = controller->GetControllerID();
    msgData->objID = mParent()->ID;

    MessagingSystem* m = gCore->GetSystem(MessagingSystem);
    m->SendMessageToSystem(msg, "NetworkingSystem");
  }
  
}
void ControllerCameraFollow::Shutdown()
{

}