#include "Standard.h"
class Object;
class IComponent;
class ObjectSystem;
class GraphicsSystem;
class JSONTranslator{
public:
  void DeserializeComponent(IComponent * component, std::ifstream & file);
  Object * CreateObjectFromFile(std::string filename);
  bool CreateFileFromObject(Object * obj);
  void SerializeObject(Object * obj, std::ofstream & file, int scope = 0);
  Object * DeserializeObject(std::ifstream & file);
  void LoadLevelFromFile(std::string filename);
  void SaveLevelToFile(std::string filename, ObjectSystem * system);
  void LoadTextures(std::string filename, GraphicsSystem * system);
};