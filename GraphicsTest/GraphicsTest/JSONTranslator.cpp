#include "JSONTranslator.h"
#include "Globals.h"
#include "Object.h"
#include "SpriteComponent.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"
#include "ObjectSystem.h"


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
  line = line.substr(line.find_first_of(':') + 2);
  obj->source = line;
  std::getline(file, line);
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->name = line;
  std::getline(file, line);
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
    obj->Initialize();
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
    if (iter != objSys->mObjects.begin()){
      file << "," << std::endl;
    }
    auto node = iter->second.head;
    while (node){
      SerializeObject(node->value,file,1);
      node = node->next;
      if (node){
        file << "," << std::endl;
      }
    }
  }
  file <<std::endl << "}";
}