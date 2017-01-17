#pragma once
#include <unordered_map>

class Object
{
  public:
  Object(): position(), scale(), rotation(), textureID(0), inUse(false){}
  float position[3];
  float scale[2];
  float rotation;
  unsigned int textureID;
  bool inUse;
  unsigned int ID;
  float r,g,b,a;
};

extern std::unordered_map<unsigned int, Object*> gObjects[10];

