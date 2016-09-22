#include "QuadTree.h"

#define SECTIONS 4

Quad::Quad() : width(0.0f), height(0.0f), midX(0.0f), midY(0.0f)
{ }

Quad::Quad(float newWidth, float newHeight, float newMidX, float newMidY) : width(newWidth), height(newHeight), midX(newMidX), midY(newMidY)
{ }

QuadTree::QuadTree(int newLevel, Quad newQuad) : level(newLevel), region(newQuad)
{ }

void QuadTree::clear()
{
  objectList.clear();
  for (auto & iter : childrenList)
  {
    if (iter != nullptr)
    {
      iter->clear();
      delete iter;
      iter = nullptr;
    }
  }
}

void QuadTree::split()
{
  Quad quadArray[SECTIONS];
  quadArray;
  for (int i = 0; i < SECTIONS; ++i)
  {
    QuadTree* child = new QuadTree(level + 1, quadArray[i]);
    childrenList.push_back(child);
  }
}