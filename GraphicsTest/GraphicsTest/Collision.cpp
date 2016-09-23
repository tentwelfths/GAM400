#include "Collision.h"

bool Collision::ResolveCollision()
{
  return true;
}

void Collision::AddObjects(Object & firObj, Object & secObj)
{
  firstObj = &firObj;
  secondObj = &secObj;
}