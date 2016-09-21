#include "JSONTranslator.h"
#include "Globals.h"
#include "Object.h"
#include "SpriteComponent.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "BoxColliderComponent.h"


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
  Object * obj = new Object;
  obj->source = filename;
  obj->name = filename;
  while (!file.eof())
  {
    std::string line;
    std::getline(file, line);
    line = trim(line);
    if (line == "{")continue;
    if (line == "}")break;
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
  file << "{" << std::endl;
  int scope = 0;
  for (auto iter = obj->mComponents.begin(); iter != obj->mComponents.end(); ++iter){
    
    file << iter->first << ": {" << std::endl;
    ++scope;
    for (auto memIter = iter->second->members.begin(); memIter != iter->second->members.end(); ++memIter){
      
      file << "\t\"" << memIter->first << "\": " << memIter->second->Get(scope);
      auto tempiter = memIter;
      tempiter++;
      if (tempiter != iter->second->members.end()){
        file << ",";
      }
      file << std::endl;
    }
    auto tempiter = iter;
    tempiter++;
    file << "}";
    if (tempiter != obj->mComponents.end()){
      file << ",";
    }
    file << std::endl;
  }
  file << "}";
  return true;
}