#include "QuadTree.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "TransformComponent.h"

#define SECTIONS 4

Quad::Quad() : width(0.0f), height(0.0f), midX(0.0f), midY(0.0f)
{ }

Quad::Quad(float newWidth, float newHeight, float newMidX, float newMidY) : width(newWidth), height(newHeight), midX(newMidX), midY(newMidY)
{ }

QuadTree::~QuadTree()
{
  for (auto & iter : childrenList)
  {
    delete iter;
  }
}

void Quad::set(float newWidth, float newHeight, float newMidX, float newMidY)
{
  width = newWidth ;
  height = newHeight;
  midX = newMidX;
  midY = newMidY;
}

bool Quad::bound(TransformComponent trans, BoxColliderComponent collider)
{
  auto pos = trans.mPosition() + collider.GetOffset();
  if (pos.x > midX - width && pos.x < midX + width && pos.y > midY - height && pos.y < midY + height)
  {
    return true;
  }
  return false;
}

bool Quad::bound(TransformComponent trans, SphereColliderComponent collider)
{
  auto pos = trans.mPosition() + collider.GetOffset();
  if (pos.x > midX - width && pos.x < midX + width && pos.y > midY - height && pos.y < midY + height)
  {
    return true;
  }
  return false;
}

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
  float newWidth = region.width / 2;
  float newHeight = region.height / 2;
  quadArray[0].set(newWidth, newHeight, region.midX - newWidth, region.midY + newHeight);
  quadArray[1].set(newWidth, newHeight, region.midX + newWidth, region.midY + newHeight);
  quadArray[2].set(newWidth, newHeight, region.midX + newWidth, region.midY - newHeight);
  quadArray[3].set(newWidth, newHeight, region.midX - newWidth, region.midY - newHeight);
  for (int i = 0; i < SECTIONS; ++i)
  {
    QuadTree* child = new QuadTree(level + 1, quadArray[i]);
    childrenList.push_back(child);
  }
}

bool QuadTree::insert(Object newMember)
{
  auto trans = newMember.GetComponentA<TransformComponent>("TransformComponent");
  auto coll = newMember.GetComponentA<BoxColliderComponent>("BoxColliderComponent");
  if (coll == nullptr)
  {
    auto coll = newMember.GetComponentA<SphereColliderComponent>("SphereColliderComponent");
    if (coll == nullptr)
    {
      return false;
    }
  }
  if (!region.bound(*trans, *coll))
  {
    return false;
  }
  if (objectList.size() < MAXOBJECT)
  {
    objectList.push_back(newMember);
    return true;
  }
  if (childrenList.empty())
  {
    split();
  }
  for (auto & iter : childrenList)
  {
    if (iter->insert(newMember))
    {
      return true;
    }
  }
  return false;
}

//int QuadTree::index(Quad area)
//{
//  return 0;
//}

bool QuadTree::retreive(std::vector<Object> possibleCollisions, Object check)
{
  auto trans = check.GetComponentA<TransformComponent>("TransformComponent");
  auto coll = check.GetComponentA<BoxColliderComponent>("BoxColliderComponent");
  if (region.bound(*trans, *coll))
  {
    if (!childrenList.empty())
    {
      for (auto & iter : childrenList)
      {
        iter->retreive(possibleCollisions, check);
      }
    }
    else
    {
      possibleCollisions = objectList;
      return true;
    }
  }
  return false;
}