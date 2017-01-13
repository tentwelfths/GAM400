#include "JSONTranslator.h"
#include "Globals.h"
#include "Object.h"
#include "Standard.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "ObjectSystem.h"
#include "GraphicsSystem.h"


void JSONTranslator::DeserializeComponent(IComponent * component, std::ifstream & file)
{
  std::string line;
  while (true)
  {
    std::getline(file,line);
    line = trim(line);
    if (line == "}" || line=="},")return;
    std::string memberName = "";
    std::string value = "";
    unsigned i = 0;
    char state = 0;
    bool complex = false;
    for (; i < line.length(); ++i){
      if (line[i] == '\"'){
        if (state == 0){
          state = 1;
        }
        else if (state == 1){
          state = 3;
        }
        continue;
      }
      else if (state == 2 && line[i] == '\"'){
        state = 3;
      }
      else if (state == 3){
        if (line[i] == ','){
          break;
        }
        else if (line[i] == '{'){
          complex = true;
        }
        else{
          if(line[i] == ':')continue;
          value += line[i];
        }
      }
      if (state == 1){
        memberName += line[i];
      }
    }
    if (complex){
      component->members[memberName]->Set(file);
      std::getline(file, line);//ending }
    }
    else{
      value = trim(value);
      component->members[memberName]->Set(value);
    }
  }
}

Object * JSONTranslator::CreateObjectFromFile(std::string filename)
{
  std::ifstream file;
  file.open(filename);
  if (!file.is_open()) return nullptr;
  Object * obj = DeserializeObject(file);
  return obj;
}

Object * JSONTranslator::DeserializeObject(std::ifstream & file)
{
  Object * obj = new Object;

  std::string line;
  std::getline(file, line);
  line = trim(line);
  if (line.find_first_of(':') != std::string::npos && line.substr(0, line.find_first_of(':')) == "ObjectID")
  {
    line = line.substr(line.find_first_of(':') + 2);
    obj->ID = std::stoi(line);
    std::getline(file, line);
  }
  else{
    obj->ID = 0;
  }
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->source = line;
  std::getline(file, line);
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->name = line;
  std::getline(file, line);
  line = trim(line);
  //if (line.find_first_of(':') != std::string::npos && line.substr(0, line.find_first_of(':')) == "ObjectID")
  //{
  //  line = line.substr(line.find_first_of(':') + 2);
  //  obj->ID = std::stoi(line);
  //  std::getline(file, line);
  //}
  while (!file.eof())
  {
    std::getline(file, line);
    line = trim(line);
    if (line == "{")continue;
    if (line == "}" || line == "},")
      break;
    line = line.substr(0, line.find_first_of(':'));
    auto * component = components[line]();
    DeserializeComponent(component, file);
    obj->AddComponent(component);
  }
  return obj;

}

bool JSONTranslator::CreateFileFromObject(Object * obj)
{
  std::ofstream file;
  file.open(obj->source);
  if (!file.is_open()) return false;
  int scope = 0;
  SerializeObject(obj, file, 0);
  return true;
}

void JSONTranslator::SerializeObject(Object* obj, std::ofstream & file, int scope)
{
  std::string tabs = "";
  for (int i = 0; i < scope; ++i){
    tabs+= "\t";
  }
  file << tabs << "File: " << obj->source << std::endl;
  file << tabs << "Name: " << obj->name << std::endl;
  file << tabs << "Object: {" << std::endl;
  for (auto iter = obj->mComponents.begin(); iter != obj->mComponents.end(); ++iter){

    file << tabs << iter->first << ": {" << std::endl;
    ++scope;
    for (auto memIter = iter->second->members.begin(); memIter != iter->second->members.end(); ++memIter){

      file << tabs << "\t\"" << memIter->first << "\": " << memIter->second->Get(scope);
      auto tempiter = memIter;
      tempiter++;
      if (tempiter != iter->second->members.end()){
        file << ",";
      }
      file << std::endl;
    }
    auto tempiter = iter;
    tempiter++;
    file << tabs << "\t}";
    if (tempiter != obj->mComponents.end()){
      file << ",";
    }
    file << std::endl;
  }
  file << tabs << "}";
}

void JSONTranslator::LoadLevelFromFile(std::string filename)
{
  std::ifstream file;
  file.open(filename);
  std::vector<Object *> levelObjs;
  if (!file.is_open()) return;
  while (!file.eof())
  {
    std::string line;
    line += file.peek();
    if (line == "{"){
      std::getline(file, line);
      continue;
    }
    if (line == "}" || line == "},")
      break;
    Object * obj = DeserializeObject(file);
    levelObjs.push_back(obj);
    obj->Register();
  }
  for (auto & iter : levelObjs){
    iter->Initialize();
  }
}

void JSONTranslator::LoadTextures(std::string filename, GraphicsSystem * system)
{
  std::ifstream file;
  file.open(filename);
  if (!file.is_open()) return;
  std::string name = "";
  GLuint index = 0;
  int frames = 1; 
  int rows = 1;
  int cols = 1;
  while (!file.eof())
  {
    std::string line;
    line += file.peek();
    if (line == "{"){
      std::getline(file, line);
      continue;
    }
    if (line == "}" || line == "},")
    {
      TextureType t;
      t.name = name; 
      t.index = index;
      t.frames = frames;
      t.rows = rows;
      t.cols = cols;
      t.textureID = system->LoadTexture("../Assets/"+name);
      system->RegisterTexture(t);
      name = "";
      index = 0;
      frames = 1;
      rows = 1;
      cols = 1;
    }

    std::getline(file, line);
    std::string propertyName;
    std::string value;
    propertyName = trim(line.substr(0, line.find_first_of(':')));
    value = trim(trim(line.substr(line.find_first_of(':') + 1)), ",");
    if (propertyName == "Name")
    {
      name = value;
    }
    else if (propertyName == "Index")
    {
      index = std::stoi(value);
    }
    else if (propertyName == "Rows")
    {
      rows = std::stoi(value);
    }
    else if (propertyName == "Cols")
    {
      cols = std::stoi(value);
    }
    else if (propertyName == "Frames")
    {
      frames = std::stoi(value);
    }
  }
}

void JSONTranslator::SaveLevelToFile(std::string filename, ObjectSystem * objSys)
{
  std::ofstream file;
  file.open(filename);
  if (!file.is_open()) return;
  file << "{" << std::endl;
  int scope = 1;
  for (auto iter = objSys->mObjects.begin(); iter != objSys->mObjects.end(); ++iter)
  {
    //if (iter != objSys->mObjects.begin()){
    //  file << "," << std::endl;
    //}
    auto node = iter->second.head;
    while (node){
      if (node->value->save)
      {
        file << "\tObjectID: " << node->value->ID << std::endl;
        SerializeObject(node->value, file, 1);
        
        //if (node){
        file << "," << std::endl;
        //}
      }
      node = node->next;
    }
  }
  file  << "}";
}