#pragma once
#include <unordered_map>

class Object
{
  public:
  Object():position(), scale(), rotation(), textureID(0) , textureName(""),inUse(false){}
  float position[3];
  float scale[3];
  float rotation[3];
  unsigned int textureID;
  std::string textureName;
  bool inUse;
};

extern std::unordered_map<unsigned int, Object*> gObjects[50];

inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
  s.erase(0, s.find_first_not_of(t));
  return s;
}

// trim from both ends of string (left & right)
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
  return ltrim(rtrim(s, t), t);
}