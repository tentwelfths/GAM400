#pragma once

#define RegisterComponentType(name) components.insert({#name, (CreateComponent<name>)})
#define AddMember(componentName, memberName) members.insert({#memberName, new Member<decltype(componentName::memberName)>((&(componentName::memberName)), Option(Options::NONE))})
#define AddSpecialMember(componentName, memberName, option) members.insert({#memberName, new Member<decltype(componentName::memberName)>((&(componentName::memberName)), option)})
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
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

enum class Options{FLOATRANGE, INTRANGE, SCROLLBOX, NONE};

struct Option{
  Option(Options o) : type(o){}
  Options type;
  float fmin, fmax;
  int imin, imax;
  std::vector<std::string> list;
};

class Mem
{
public:
  Mem(Option o) : myOp(o){}
  virtual void Set(std::string) = 0;
  virtual void Set(std::ifstream &file) = 0;
  virtual std::string Get(int scope) = 0;
  virtual void GetUI(std::string name) = 0;
  Option myOp;
};

template<typename T>
class Member : public Mem
{
public:
  T * val;
  typedef T myType;
  Member(T * v, Option o) : Mem(o),val(static_cast<T *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = value;
  }
  std::string Get(int scope){
    return std::string(*val);
  }
  void GetUI(std::string name){
    ImGui::InputText(name.c_str(), val, IM_ARRAYSIZE(val));
  }
};

template<>
class Member<std::string> : public Mem
{
public:
  std::string * val;
  typedef std::string myType;
  char temp[64];
  Member(std::string * v, Option o) : Mem(o), val(static_cast<std::string *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = value;
  }
  std::string Get(int scope){
    return std::string(*val);
  }
  void GetUI(std::string name){
    //std::array<char, 128> arr = (const_cast<char *>(val->data()));
    //if (val->size() <= val->length() + 2){
    //  val->resize(val->size() * 2);
    //}
    if (myOp.type == Options::SCROLLBOX){
      const char* items[128];
      unsigned i = 0;
      for (auto & iter : myOp.list){
        items[i] = iter.c_str();
        ++i;
      }
      for (i = 0; i < myOp.list.size(); ++i){
        if (myOp.list[i] == *val) break;
      }
      static int item2 = i;
      ImGui::Combo(name.c_str(), &item2, items, myOp.list.size());

      *val = myOp.list[item2];
    }
    else{
      strcpy(temp, val->c_str());
      ImGui::InputText(name.c_str(), temp, 64);
      //*val = (arr.data());
      *val = temp;
    }
  }
};

template<>
class Member<int> : public Mem
{
public:
  int * val;
  typedef int myType;
  Member(int * v, Option o) : Mem(o), val(static_cast<int *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::atoi(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
  void GetUI(std::string name){
    ImGui::InputInt(name.c_str(), val);
  }
};

template<>
class Member<glm::vec3> : public Mem
{
public:
  glm::vec3 * val;
  typedef glm::vec3 myType;
  Member(glm::vec3 * v, Option o) : Mem(o), val(static_cast<glm::vec3 *>(v)) {}
  void Set(std::string str)
  {
    std::string var[3], nval[3], line[3];
    int count = -1;
    for (unsigned i = 0; i < str.length(); ++i){
      if (count >= 0 && count < 3)
        line[count] += str[i];
      if (str[i] == '\n')++count;
    }
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
  void GetUI(std::string name){
    ImGui::Text(name.c_str());
    std::string x = "x##" + name;
    std::string y = "y##" + name;
    std::string z = "z##" + name;
    if (myOp.type == Options::FLOATRANGE){
      ImGui::SliderFloat(x.c_str(), &(val->x), myOp.fmin, myOp.fmax, "%.3f");
      ImGui::SliderFloat(y.c_str(), &(val->y), myOp.fmin, myOp.fmax, "%.3f");
      ImGui::SliderFloat(z.c_str(), &(val->z), myOp.fmin, myOp.fmax, "%.3f");
    }
    else{
      ImGui::InputFloat(x.c_str(), &(val->x));
      ImGui::InputFloat(y.c_str(), &(val->y));
      ImGui::InputFloat(z.c_str(), &(val->z));
    }
  }
};

template<>
class Member<unsigned> : public Mem
{
public:
  unsigned * val;
  typedef unsigned myType;
  Member(unsigned * v, Option o) : Mem(o), val(static_cast<unsigned *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::stoi(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
  void GetUI(std::string name){
    ImGui::InputInt(name.c_str(), (int*)val);
  }
};

template<>
class Member<double> : public Mem
{
public:
  double * val;
  typedef double myType;
  Member(double * v, Option o) : Mem(o), val(static_cast<double *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::stod(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
  void GetUI(std::string name){
    if (myOp.type == Options::FLOATRANGE){
      ImGui::SliderFloat(name.c_str(), reinterpret_cast<float*>(val), myOp.fmin, myOp.fmax, "%.3f");
    }
    else{
      ImGui::InputFloat(name.c_str(), reinterpret_cast<float*>(val));
    }
  }
};

template<>
class Member<float> : public Mem
{
public:
  float * val;
  typedef float myType;
  Member(float * v, Option o) : Mem(o), val(static_cast<float *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = std::stof(value.c_str());
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
  void GetUI(std::string name){
    if (myOp.type == Options::FLOATRANGE){
      ImGui::SliderFloat(name.c_str(), (val), myOp.fmin, myOp.fmax, "%.3f");
    }
    else{
      ImGui::InputFloat(name.c_str(), (val));
    }
  }
};

template<>
class Member<char> : public Mem
{
public:
  char * val;
  typedef char myType;
  Member(char * v, Option o) : Mem(o), val(static_cast<char *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = value[0];
  }
  std::string Get(int scope){
    return std::to_string(*val);
  }
  void GetUI(std::string name){
    ImGui::InputText(name.c_str(), val, 1);
  }
};

template<>
class Member<bool> : public Mem
{
public:
  bool * val;
  typedef bool myType;
  Member(bool * v, Option o) : Mem(o), val(static_cast<bool *>(v)) {}
  void Set(std::ifstream &file){};
  void Set(std::string value){
    *val = (value == "true");
  }
  std::string Get(int scope){
    return ((*val) ? "true" : "false");
  }
  void GetUI(std::string name){
    ImGui::Checkbox(name.c_str(), val);
  }
};