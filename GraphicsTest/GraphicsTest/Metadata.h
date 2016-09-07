#define RegisterComponentType(name) components.insert({#name, (CreateComponent<name>)})
#define AddMember(componentName, memberName) members.insert({#memberName, new Member<decltype(componentName::memberName)>((&(componentName::memberName)))})


template<typename T>
IComponent * CreateComponent(){
  return new T;
}

class Mem
{
public:
  virtual void Set(std::string) = 0;
  virtual void Set(std::ifstream &file)
};

template<typename T>
class Member : public Mem
{
public:
  T * val;
  Member(T * v) : val(static_cast<T *>(v)) {}
  void Set(std::string value){
    *value = value;
  }
};

template<>
class Member<int> : public Mem
{
public:
  int * val;
  Member(int * v) : val(static_cast<int *>(v)) {}
  void Set(std::string value){
    *val = std::atoi(value.c_str());
  }
};

template<>
class Member<glm::vec3> : public Mem
{
public:
  glm::vec3 * val;
  Member(glm::vec3 * v) : val(static_cast<glm::vec3 *>(v)) {}
  void Set(std::string){}
  void Set(std::ifstream &file){
    std::string var[3], nval[3], line[3];

    std::getline(file, line[1]);
    std::getline(file, line[2]);
    std::getline(file, line[3]);
    for (int j = 0; j < 3; ++j){
      unsigned i = 0;
      for (; i < line[j].length(); ++i){
        if (line[j][i] == ':') break;
        var[j] += line[j][i];
      }
      
      nval[j] = line[j].substr(i);
      if (nval[j][nval[j].length() - 1] == ','){
        nval[j] = nval[j].substr(0, nval[j].length() - 1);
      }
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
};

template<>
class Member<unsigned> : public Mem
{
public:
  unsigned * val;
  Member(unsigned * v) : val(static_cast<unsigned *>(v)) {}
  void Set(std::string value){
    *val = std::atoi(value.c_str());
  }
};

template<>
class Member<double> : public Mem
{
public:
  double * val;
  Member(double * v) : val(static_cast<double *>(v)) {}
  void Set(std::string value){
    *val = std::atof(value.c_str());
  }
};

template<>
class Member<float> : public Mem
{
public:
  float * val;
  Member(float * v) : val(static_cast<float *>(v)) {}
  void Set(std::string value){
    *val = std::atof(value.c_str());
  }
};

template<>
class Member<char> : public Mem
{
public:
  char * val;
  Member(char * v) : val(static_cast<char *>(v)) {}
  void Set(std::string value){
    *val = value[0];
  }
};