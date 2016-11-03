#include <Box2D\Box2D.h>
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
#include "DemoSpawner.h"
#include "InputSystem.h"
#include "BoxColliderComponent.h"

#define ROCKSPEED 5.0f
#define ROCKMULTI 1.5f
#define CLICKY 6.0f

DemoSpawner::DemoSpawner() : GameLogicComponent(GameLogicType::DEMODROPPER), leftCD(0.5f), rightCD(1.0f), theLeftCD(0.5f), theRightCD(1.0f)
{
  mName_ = "DemoSpawner";
}

bool DemoSpawner::Initialize()
{
  return true;
}

void DemoSpawner::Update(double dt)
{
  auto * input = gCore->GetSystem(InputSystem);
  if (input->isKeyPressed(GLFW_MOUSE_BUTTON_1) && theLeftCD > leftCD)
  {
    JSONTranslator j;
    Object * b;
    b = j.CreateObjectFromFile("DemoRock.json");
    //b->AddComponent(new RigidbodyComponent);
    //b->AddComponent(new BoxColliderComponent);
    b->name = "Hazard";
    b->Register();
    b->Initialize();


    auto * trans2 = b->GetComponent(TransformComponent);
    trans2->mPosition_.y = input->GetMouseY();
    trans2->mPosition_.x = input->GetMouseX();
    auto* col = b->GetComponent(BoxColliderComponent);
    b2Vec2 newPos(trans2->mPosition_.x + col->GetOffset().x, CLICKY + col->GetOffset().y);
    col->GetBody()->SetTransform(newPos, trans2->mRotationZ());
    b2Vec2 newVel(0.0f,-ROCKSPEED);
    col->GetBody()->SetLinearVelocity(newVel);
    theLeftCD = 0.0f;
  }
  if (input->isKeyPressed(GLFW_MOUSE_BUTTON_2) && theRightCD > rightCD)
  {
    JSONTranslator j;
    Object * b;
    b = j.CreateObjectFromFile("DemoGiantRock.json");
    //b->AddComponent(new RigidbodyComponent);
    //b->AddComponent(new BoxColliderComponent);
    b->name = "Hazard";
    b->Register();
    b->Initialize();


    auto * trans2 = b->GetComponent(TransformComponent);
    trans2->mPosition_.y = input->GetMouseY();
    trans2->mPosition_.x = input->GetMouseX();
    auto* col = b->GetComponent(BoxColliderComponent);
    b2Vec2 newPos(trans2->mPosition_.x + col->GetOffset().x, CLICKY + col->GetOffset().y);
    col->GetBody()->SetTransform(newPos, trans2->mRotationZ());
    b2Vec2 newVel(0.0f, -ROCKSPEED * ROCKMULTI);
    col->GetBody()->SetLinearVelocity(newVel);
    theRightCD = 0.0f;
  }
  theLeftCD  += (float)dt;
  theRightCD += (float)dt;
}

void DemoSpawner::Shutdown()
{

}