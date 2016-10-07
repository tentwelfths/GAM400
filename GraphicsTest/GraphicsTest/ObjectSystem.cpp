#include "Object.h"
#include "ObjectSystem.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

ObjectSystem::ObjectSystem()
{
  mName_ = "ObjectSystem";
  myCount = 0;
  numObjects = 0;
}

void     ObjectSystem::RegisterComponent(IComponent*)
{

}

bool     ObjectSystem::Initialize()
{
  return true;
}

Object * ObjectSystem::CreateObjectFromFile(std::string filename){
  return nullptr;
}
void ObjectSystem::CreateArchetypeFromObject(Object * obj){

}

void ObjectSystem::ClearSystem(){
  for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
  {
    auto node = iter->second.head;
    while (node){
      node->value->Destroy();
      node = node->next;
    }
  }
}


std::string ObjectSystem::GetData(Object * iter)
{
  TransformComponent * t = iter->GetComponent(TransformComponent);
  std::string data = "`";
  int i = 0;
  ++i;
  for (int k = 0; k < sizeof(unsigned int); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(iter->ID)))[k];
    ++i;
  }
  unsigned char len = (static_cast<SpriteComponent *>(iter->GetComponent(SpriteComponent))->mTextureName.length());
  data += len;
  for (unsigned char i = 0; i < len; ++i){
    data += (static_cast<SpriteComponent *>(iter->GetComponent(SpriteComponent))->mTextureName[i]);
  }
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mPosition_.x)))[k];
    ++i;
  }
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mPosition_.y)))[k];
    ++i;
  }
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mPosition_.z)))[k];
    ++i;
  }
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mScale_.x)))[k];
    ++i;
  }
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mScale_.y)))[k];
    ++i;
  }
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mRotation_.z)))[k];
    ++i;
  }
  //std::cout << "LENGTH: " << i << " ---" << frameData.length() << std::endl;
  return data;
}

void     ObjectSystem::Update(double dt)
{
  frameData = "";
  for (auto iter = deadObjects.begin(); iter != deadObjects.end(); ++iter){
    if (iter->second <= 1)
    {
      //std::cout << "FUCK YOU BE DIE" << iter->first << std::endl;
      frameData += '%';
      for (int k = 0; k < sizeof(unsigned int); ++k)
      {
          frameData += static_cast<const char *>(static_cast<const void *>(&(iter->first)))[k];
      }
    }
    if (iter->second == 0){
      iter->second = 30;
    }
    iter->second -= 1;
  }
  for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
  {
    auto node = iter->second.head;
    while (node){
      //node->value->hasChanged = true;
      
      if (node->value->dead){
        bool removable = true;
        for (auto compiter = node->value->mComponents.begin(); compiter != node->value->mComponents.end();){
          
          if (compiter->second->clean == false){
            removable = false;
            ++compiter;
          }
          else{
            IComponent * temp = compiter->second;
            compiter = node->value->mComponents.erase(compiter);
            temp->Shutdown();
          }
        }
        if (removable){
          deadObjects.insert({ node->value->ID, 1 });
          --numObjects;
          delete node->value;
          node = iter->second.Remove(node);
        }
        else{
          node = node->next;
        }
        continue;
      }
      if (node->value->hasChanged || node->value->age > rand() % 100 + 100){
        frameData += GetData(node->value);
        if (node->value->age == 300){
          node->value->age = 0;
        }
        else{
          node->value->age = 300;
        }
        
        node->value->hasChanged = false;
      }
      else{
        //std::cout << node->value->age << std::endl;
        node->value->age += 1;
      }
      node = node->next;
    }
  }
}

void     ObjectSystem::Shutdown()
{

}

void ObjectSystem::AddObject(Object * obj)
{
  obj->ID = myCount++;
  obj->hasChanged = true;
  if (mObjects.find(obj->name) == mObjects.end())
  {
    mObjects.insert({ obj->name, List<Object*>() });
  }
  mObjects[obj->name].AddToFront(obj);
  ++numObjects; 
}

Object * ObjectSystem::GetFirstItemByName(std::string name)
{
  if (!mObjects[name].head)return nullptr;
  return mObjects[name].head->value;
}
Object * ObjectSystem::GetNthItemByName(std::string name, unsigned n)
{
  unsigned i = 0;
  auto head = mObjects[name].head;
  while (i < n && head)
  {
    ++i;
    head = head->next;
  }
  if (!head)return nullptr;
  return head->value;
}
void ObjectSystem::RemoveDeadObject(unsigned int ID)//FUCK YOU THIS DOESN'T DO WHAT YOU THINK IT DOES. IS FOR NATWURKN
{
  for (auto iter = deadObjects.begin(); iter != deadObjects.end(); ++iter){
    if (iter->first == ID){
      deadObjects.erase(iter);
      return;
    }
  }
}
