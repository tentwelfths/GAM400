#include "Standard.h"
class Object;
class IComponent;
class JSONTranslator{
  void DeserializeComponent(IComponent * component, std::ifstream & file);
  Object * CreateObjectFromFile(std::string filename);
  bool CreateFileFromObject(Object * obj);
};