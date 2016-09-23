#pragma once
#include "Standard.h"
#include "Globals.h"
#include "Object.h"
#include <vector>

class TransformComponent;
class BoxColliderComponent;
class SphereColliderComponent;

#define MAXOBJECT 5
#define MAXCHILDREN 4
#define MAXLEVELS 6

struct Quad
{
  Quad();
  Quad(float newWidth, float newHeight, float newMidX, float newMidY);
  float width;
  float height;
  float midX;
  float midY;
  void set(float newWidth, float newHeight, float newMidX, float newMidY);
  bool bound(TransformComponent& trans, BoxColliderComponent& collider);
  bool bound(TransformComponent& trans, SphereColliderComponent& collider);
};


class QuadTree
{
  public:
    //QuadTree();
    QuadTree(int newLevel, Quad newQuad);
    ~QuadTree();
    void clear();
    void split();
    bool insert(Object& newMember);
    //int index(Quad area);
    bool retreive(std::vector<Object>& possibleCollisions, Object check);
    void setActive(bool newActive) { active = newActive; }
    void createChildren();
  private:
    bool active;
    int level;
    Quad region;
    QuadTree* childrenList[MAXCHILDREN];
    std::vector<Object> objectList;

};