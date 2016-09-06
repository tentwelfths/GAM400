// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include "Core.h"
#include "Object.h"
#include "Globals.h"
#include <iostream>
#include "SpriteComponent.h"
#include "TransformComponent.h"

Core * gCore = nullptr;

int main( void )
{
  gCore = new Core();
  gCore->Initialize();
  Object a, b, c;
  a.AddComponent(new TransformComponent());
  a.AddComponent(new SpriteComponent());
  TransformComponent * t = a.GetComponent(TransformComponent);
  SpriteComponent * s = a.GetComponent(SpriteComponent);
  s->SetTexture("A.png");
  t->mPosition(0, 0, 0);
  t->mScale(1.f, 2.f, 1.f);

  b.AddComponent(new TransformComponent());
  b.AddComponent(new SpriteComponent());
  t = b.GetComponent(TransformComponent);
  s = b.GetComponent(SpriteComponent);
  s->SetTexture("Kakka_Carrot_Veggie.png");
  t->mPosition(-3, 2, 0);
  t->mScale(3.f, 5.f, 1.f);

  c.AddComponent(new TransformComponent());
  c.AddComponent(new SpriteComponent());
  t = c.GetComponent(TransformComponent);
  s = c.GetComponent(SpriteComponent);
  s->SetTexture("HealSpell.png");
  t->mPosition(4, -2, 0);
  t->mScale(1.4f, 1.1, 1.f);

  while (true)
  {
    gCore->Update(0.0016);
   // t->mPositionX(t->mPositionX() + 0.1);
  }

	return 0;
}

