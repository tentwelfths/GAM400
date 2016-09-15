#pragma once
#include "Standard.h"

class Object;

class Collision
{
  public:
    bool ResolveCollision();
    void AddObjects(Object * firObj, Object * secObj);
  private:
    Object * firstObj;
    Object * secondObj;
};