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
#include "BoxColliderComponent.h"
#include "EditorComponent.h"
#include "DemoController.h"
#include "DemoSpawner.h"
#include "OutOfBounds.h"
#include "DemoReload.h"
#include "PCControllerComponent.h"
#include "KnifeComponent.h"
#include "ConeControllerComponent.h"
#include "ConeComponent.h"
#include "BulletComponent.h"
#include "StartManagerComponent.h"
#include "LevelManagerComponent.h"
#include "PCCameraFollow.h"
#include "ControllerCameraFollow.h"
#include "GoHomeComponent.h"
#include "CloudComponent.h"
#include "GunControllerComponent.h"

Core * gCore = nullptr;
std::unordered_map<std::string, IComponent*(*)()> components;

//int f(int n){
//  if (n>10000) return 0;
//  return (n*n) / (6 + f(n + 2));
//}
//
//float fIter(float n)
//{
//  float total = 0;
//  float max = n;
//  while (max < 9998) max += 2;
//  float denom = max * max / 6.f;
//  while (max >= n){
//    denom = max * max / (6.f + denom);
//    max -= 2;
//  }
//  return denom;
//}

int main( void )
{
  //std::cout << 3 + fIter(1);
  //std::cin.get();
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
  RegisterComponentType(ConeComponent);
  RegisterComponentType(BulletComponent);
  RegisterComponentType(StartManagerComponent);
  RegisterComponentType(LevelManagerComponent);
  RegisterComponentType(PCCameraFollow);
  RegisterComponentType(ControllerCameraFollow);
  RegisterComponentType(GoHomeComponent);
  RegisterComponentType(CloudComponent);
  RegisterComponentType(GunControllerComponent);

  gCore->LoadLevel("DemoLevel.json");

  while (true)
  {
    gCore->Update(0.0016);
  }

	return 0;
}


