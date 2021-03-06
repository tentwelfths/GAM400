#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Metadata.h"
#include "System.h"
#include "IComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "InputSystem.h"

PlayerControllerComponent::PlayerControllerComponent() : GameLogicComponent(GameLogicType::PLAYER), alive(true), speed(1.1f), playerID(0), maxHealth(2), currHealth(2), killable(false)
{
  AddMember(PlayerControllerComponent, speed);
  AddMember(PlayerControllerComponent, playerID);
  AddMember(PlayerControllerComponent, maxHealth);
  AddMember(PlayerControllerComponent, killable);
  mName_ = "PlayerControllerComponent";
}

bool PlayerControllerComponent::Initialize()
{
  return true;
}

void PlayerControllerComponent::Update(double dt)
{
  //auto * input = gCore->GetSystem(InputSystem);
  //auto * rigid = mParent_->GetComponent(BoxColliderComponent);
  //b2Vec2 newVel(0.0f,0.0f);
  //if (rigid)
  //{
  //  Joystick j = input->getJoystick(0);
  //  if (input->isButtonJustPressed(0, 0))
  //  {
  //    
  //    JSONTranslator j;
  //    Object * b;
  //    b = j.CreateObjectFromFile("B.json");
  //    //b->AddComponent(new RigidbodyComponent);
  //    //b->AddComponent(new BoxColliderComponent);
  //    b->name = "Fuccboi2";
  //    b->Initialize();
  //  
  //  
  //    auto * trans2 = b->GetComponent(TransformComponent);
  //    trans2->mPosition_.y = (rand()%100 - 50) / 10.f;
  //    trans2->mPosition_.x = (rand()%100 - 50) / 10.f;
  //    auto* col = b->GetComponent(BoxColliderComponent);
  //    b2Vec2 newPos(trans2->mPosition_.x + col->GetOffset().x, trans2->mPosition_.y + col->GetOffset().y);
  //    col->GetBody()->SetTransform(newPos, trans2->mRotationZ());
  //  }
  //  if (j.xStick > 0.1 || j.xStick < -0.1){
  //    newVel.x = j.xStick * speed;
  //  }
  //  if (j.yStick > 0.1 || j.yStick < -0.1){
  //    newVel.y = j.yStick * speed;
  //  }
  //  
  //  if (input->isKeyPressed(GLFW_KEY_W))
  //  {
  //    //newVel.y = speed;
  //    JSONTranslator j;
  //    Object * b;
  //    b = j.CreateObjectFromFile("B.json");
  //    //b->AddComponent(new RigidbodyComponent);
  //    //b->AddComponent(new BoxColliderComponent);
  //    b->name = "Fuccboi2";
  //    b->Initialize();
  //    
  //    
  //    auto * trans2 = b->GetComponent(TransformComponent);
  //    trans2->mPosition_.y = (rand() % 100 - 50) / 10.f;
  //    trans2->mPosition_.x = (rand() % 100 - 50) / 10.f;
  //    auto* col = b->GetComponent(BoxColliderComponent);
  //    b2Vec2 newPos(trans2->mPosition_.x + col->GetOffset().x, trans2->mPosition_.y + col->GetOffset().y);
  //    col->GetBody()->SetTransform(newPos, trans2->mRotationZ());
  //    //col->GetType()->position = newPos;
  //    //int c = 10;
  //  }
  //  if (input->isKeyPressed(GLFW_KEY_D))
  //  {
  //    newVel.x = speed;
  //  }
  //  //else if (input->isKeyPressed(GLFW_KEY_S))
  //  //{
  //  //  newVel.y = -speed;
  //  //}
  //  //if (input->isKeyPressed(GLFW_KEY_A))
  //  //{
  //  //  newVel.x = -speed;
  //  //}
  //  //else if (input->isKeyPressed(GLFW_KEY_D))
  //  //{
  //  //  newVel.x = speed;
  //  //}
  //  if (rigid->GetBody())
  //  {
  //    rigid->GetBody()->SetLinearVelocity(newVel);
  //  }
  //}
}

void PlayerControllerComponent::Shutdown()
{
}

void PlayerControllerComponent::Movement(InputSystem* input)
{

}