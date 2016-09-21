#include <stdio.h>
#include <stdlib.h>
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include <iostream>
#include "Metadata.h"
#include "System.h"
#include "IComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "JSONTranslator.h"
#include "PlayerControllerComponent.h"
#include "RigidbodyComponent.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"

PlayerControllerComponent::PlayerControllerComponent() : GameLogicComponent(GameLogicType::PLAYER), speed(1.1f)
{
  AddMember(PlayerControllerComponent, speed);
  mName_ = "PlayerControllerComponent";
}

bool PlayerControllerComponent::Initialize()
{
  return true;
}

void PlayerControllerComponent::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  auto * rigid = mParent_->GetComponent(RigidbodyComponent);
  vec3 newVel;
  if (rigid)
  {
    Joystick j = input->getJoystick(0);
    std::cout << "THE BUTTON STATE IS: " << input->isButtonJustPressed(0, 0) << std::endl;
    if (input->isButtonJustPressed(0, 0))
    {
      
      JSONTranslator j;
      Object * b;
      b = j.CreateObjectFromFile("B.json");
      b->AddComponent(new RigidbodyComponent);
      b->AddComponent(new BoxColliderComponent);
      b->name = "Fuccboi2";
      b->Initialize();
    
    
      auto * trans2 = b->GetComponent(TransformComponent);
      trans2->mPosition_.y = (rand()%100 - 50) / 10.f;
      trans2->mPosition_.x = (rand()%100 - 50) / 10.f;
    }
    if (j.xStick > 0.1 || j.xStick < -0.1){
      newVel.x = j.xStick * speed;
    }
    if (j.yStick > 0.1 || j.yStick < -0.1){
      newVel.y = j.yStick * speed;
    }
    
    //if (input->isKeyPressed(GLFW_KEY_W))
    //{
    //  newVel.y = speed;
    //}
    //else if (input->isKeyPressed(GLFW_KEY_S))
    //{
    //  newVel.y = -speed;
    //}
    //if (input->isKeyPressed(GLFW_KEY_A))
    //{
    //  newVel.x = -speed;
    //}
    //else if (input->isKeyPressed(GLFW_KEY_D))
    //{
    //  newVel.x = speed;
    //}
    rigid->SetVel(newVel);
  }

}

void PlayerControllerComponent::Shutdown()
{

}