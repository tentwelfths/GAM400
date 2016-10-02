#pragma once

#define RegisterComponentType(name) components.insert({#name, (CreateComponent<name>)})
#define AddMember(componentName, memberName) members.insert({#memberName, new Member<decltype(componentName::memberName)>((&(componentName::memberName)))})
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Standard.h"
/*
class Memm
{
public:
  Memm(){}
  virtual void Set(std::string) {}
  virtual void Set(std::ifstream &file) {}
  virtual std::string Get() { return std::string(""); }
};
*/
class Mem
{
public:
  Mem(){}
  virtual void Set(std::string) = 0;
  virtual void Set(std::ifstream &file) = 0;
  virtual std::string Get(int scope) = 0;
};

template<typename T>
class Member : public Mem
{
public:
  T * val;
  Member(T * v) : Mem(),val(static_cast<T *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = value;
  }
  std::string Get(int scope){
    return std::string(*val);
  }
};

template<>
class Member<int> : public Mem
{
public:
  int * val;
  Member(int * v) : Mem(), val(static_cast<int *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::atoi(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
};

template<>
class Member<glm::vec3> : public Mem
{
public:
  glm::vec3 * val;
  Member(glm::vec3 * v) : Mem(), val(static_cast<glm::vec3 *>(v)) {}
  void Set(std::string){}
  void Set(std::ifstream &file){
    std::string var[3], nval[3], line[3];

    std::getline(file, line[0]);
    std::getline(file, line[1]);
    std::getline(file, line[2]);
    for (int j = 0; j < 3; ++j){
      unsigned i = 0;
      for (; i < line[j].length(); ++i){
        if (line[j][i] == ':') break;
        var[j] += line[j][i];
      }
      
      nval[j] = line[j].substr(i + 1);
      if (nval[j][nval[j].length() - 1] == ','){
        nval[j] = nval[j].substr(0, nval[j].length() - 1);
      }
      var[j] = trim(var[j]);
      nval[j] = trim(nval[j]);
      if (var[j] == "x"){
        val->x = std::stof(nval[j]);
      }
      if (var[j] == "y"){
        val->y = std::stof(nval[j]);
      }
      if (var[j] == "z"){
        val->z = std::stof(nval[j]);
      }
    }
  }
  std::string Get(int scope){
    std::string s = "{\n";
    std::string tabs = "";
    for (int i = 0; i <= scope; ++i) tabs += "\t";
    s += tabs + "x: " + std::to_string(val->x);
    s += ",\n" + tabs + "y: " + std::to_string(val->y);
    s += ",\n" + tabs + "z: " + std::to_string(val->z);
    s += "\n" + tabs + "}";
    return s;
  }
};

template<>
class Member<unsigned> : public Mem
{
public:
  unsigned * val;
  Member(unsigned * v) : Mem(), val(static_cast<unsigned *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::stoi(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
};

template<>
class Member<double> : public Mem
{
public:
  double * val;
  Member(double * v) :Mem(), val(static_cast<double *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::stod(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
};

template<>
class Member<float> : public Mem
{
public:
  float * val;
  Member(float * v) :Mem(), val(static_cast<float *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::stof(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
};

template<>
class Member<char> : public Mem
{
public:
  char * val;
  Member(char * v) :Mem(), val(static_cast<char *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = value[0];
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
};

template<>
class Member<bool> : public Mem
{
public:
  bool * val;
  Member(bool * v) :Mem(), val(static_cast<bool *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = (value == "true");
  }
  std::string Get(int scope){
    return ((*val) ? "true" : "false");
  }
};