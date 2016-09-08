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

Core * gCore = nullptr;
std::unordered_map<std::string, IComponent*(*)()> components;

int main( void )
{
  JSONTranslator j;
  //components.insert({ "SpriteComponent", (CreateComponent<SpriteComponent>) });
  RegisterComponentType(SpriteComponent);
  RegisterComponentType(TransformComponent);
  gCore = new Core();
  gCore->Initialize();
  Object * a, *b, *c;
  a = j.CreateObjectFromFile("A.json");
  b = j.CreateObjectFromFile("B.json");
  c = j.CreateObjectFromFile("C.json");
  while (true)
  {
    gCore->Update(0.0016);
   // t->mPositionX(t->mPositionX() + 0.1);
  }

	return 0;
}

