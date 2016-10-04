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

DemoSpawner::DemoSpawner() :GameLogicComponent(GameLogicType::DEMODROPPER)
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
  if (input->isKeyPressed(GLFW_MOUSE_BUTTON_1))
  {
    int b = 10;
  }
}

void DemoSpawner::Shutdown()
{

}