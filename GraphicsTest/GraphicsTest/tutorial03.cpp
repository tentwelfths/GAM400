// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include <iostream>
#include "Metadata.h"
#include "IComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "JSONTranslator.h"
#include "PlayerControllerComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "EditorComponent.h"
#include "DemoController.h"
#include "DemoSpawner.h"
#include "OutOfBounds.h"
#include "DemoReload.h"
#include "PCControllerComponent.h"
#include "KnifeComponent.h"
#include "ConeControllerComponent.h"

Core * gCore = nullptr;
std::unordered_map<std::string, IComponent*(*)()> components;

int main( void )
{
  gCore = new Core();
  gCore->Initialize();
  RegisterComponentType(SpriteComponent);
  RegisterComponentType(TransformComponent);
  RegisterComponentType(BoxColliderComponent);
  RegisterComponentType(PlayerControllerComponent);
  RegisterComponentType(DemoController);
  RegisterComponentType(DemoSpawner);
  RegisterComponentType(EditorComponent);
  RegisterComponentType(OutOfBounds);
  RegisterComponentType(DemoReload);
  RegisterComponentType(PCControllerComponent);
  RegisterComponentType(KnifeComponent);
  RegisterComponentType(ControllerControllerComponent);
  RegisterComponentType(ConeControllerComponent);

  gCore->LoadLevel("DemoLevel.json");

  while (true)
  {
    gCore->Update(0.0016);
  }

	return 0;
}

