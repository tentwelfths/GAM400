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

Core * gCore = nullptr;
std::unordered_map<std::string, IComponent*(*)()> components;

int main( void )
{
  JSONTranslator j;
  //components.insert({ "SpriteComponent", (CreateComponent<SpriteComponent>) });
  RegisterComponentType(SpriteComponent);
  RegisterComponentType(TransformComponent);
  RegisterComponentType(RigidbodyComponent);
  RegisterComponentType(BoxColliderComponent);
  RegisterComponentType(PlayerControllerComponent);
  RegisterComponentType(EditorComponent);
  gCore = new Core();
  gCore->Initialize();
  //Object * a;
  //a = j.CreateObjectFromFile("Player.json");
  //a->name = "Fuccboi";
  //a->Initialize();
  //
  //auto box1 = a->GetComponent(BoxColliderComponent);
  //box1->SetHalfSize(vec3(0.5f, 0.5f, 1.0f));
  //
  //Object * b;
  //
  //b = j.CreateObjectFromFile("B.json");
  //b->Initialize();
  //j.CreateFileFromObject(b);
  //j.SaveLevelToFile("TestLevel.json", gCore->GetSystem(ObjectSystem));
  j.LoadLevelFromFile("TestLevel.json");
  //return 0;
  while (true)
  {
    gCore->Update(0.0016);
   // t->mPositionX(t->mPositionX() + 0.1);
  }

	return 0;
}

