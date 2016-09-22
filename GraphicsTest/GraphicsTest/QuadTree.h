#pragma once
#include "Standard.h"
#include "Globals.h"
#include "Object.h"
#include <vector>

#define MAXOBJECT 5
struct Quad
{
  Quad();
  Quad(float newWidth, float newHeight, float newMidX, float newMidY);
  float width;
  float height;
  float midX;
  float midY;
  void set(float newWidth, float newHeight, float newMidX, float newMidY);
};


class QuadTree
{
  public:
    //QuadTree();
    QuadTree(int newLevel, Quad newQuad);
    void clear();
    void split();
    void insert(Object newMember);
    int index(Quad area);
    std::vector<Object> retreive(std::vector<Object> possibleCollisions, Object check);
  private:
    int level;
    Quad region;
    std::vector<QuadTree*> childrenList;
    std::vector<Object> objectList;

};