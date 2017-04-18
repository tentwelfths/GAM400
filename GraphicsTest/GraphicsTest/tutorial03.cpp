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
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "EditorComponent.h"
#include "OutOfBounds.h"
#include "StartManagerComponent.h"
#include "GoHomeComponent.h"
#include "PuzzlePi.h"
#include "ResetObj.h"
#include "PuzzleRightStick.h"
#include "PuzzleObjectSpawn.h"
#include "PuzzleObject.h"
#include "PuzzleHints.h"
#include "PCCameraFollow.h"
#include "TimerManagerComponent.h"
#include "KeypadButtonComponent.h"
#include "KeypadComboManager.h"

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
  RegisterComponentType(CircleColliderComponent);
  RegisterComponentType(EditorComponent);
  RegisterComponentType(OutOfBounds);
  RegisterComponentType(StartManagerComponent);
  RegisterComponentType(GoHomeComponent);
  RegisterComponentType(PuzzlePi);
  RegisterComponentType(ResetObj);
  RegisterComponentType(PuzzleRightStick);
  RegisterComponentType(PuzzleObjectSpawn);
  RegisterComponentType(PuzzleObject);
  RegisterComponentType(PuzzleHints);
  RegisterComponentType(PCCameraFollow);
  RegisterComponentType(TimerManagerComponent);
  RegisterComponentType(KeypadButtonComponent);
  RegisterComponentType(KeypadComboManager);

  gCore->LoadLevel("MainMenu.json");

  while (gCore->active)
  {
    gCore->Update(0.0016);
  }
  gCore->UnloadLevel();

	return 0;
}


