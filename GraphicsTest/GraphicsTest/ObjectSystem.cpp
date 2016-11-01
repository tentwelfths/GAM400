#include "Object.h"
#include "ObjectSystem.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Core.h"
#include "GraphicsSystem.h"
#include "Globals.h"
#include "NetworkingSystem.h"
#include "JSONTranslator.h"

ObjectSystem::ObjectSystem()
{
  mName_ = "ObjectSystem";
  myCount = 1;
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
  JSONTranslator j;
  Object * obj = j.CreateObjectFromFile(filename);
  if (obj != nullptr){
    obj->Initialize();
  }
  return obj;
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

std::string ObjectSystem::GetData(unsigned int ID)
{
  return GetData(mObjectMap_[ID]);
}

std::string ObjectSystem::GetTransformData(unsigned int ID)
{
  return GetTransformData(mObjectMap_[ID]);
}

std::string ObjectSystem::Get2DPositionData(unsigned int ID)
{
  return Get2DPositionData(mObjectMap_[ID]);
}

std::string ObjectSystem::GetTextureData(unsigned int ID)
{
  return GetTextureData(mObjectMap_[ID]);
}


std::string ObjectSystem::Get2DPositionData(Object * iter)
{
  if (iter == nullptr || iter->dead) return "";
  TransformComponent * t = iter->GetComponent(TransformComponent);
  std::string data = "";
  //for (int k = 0; k < sizeof(unsigned int); ++k)
  //{
  //  data += static_cast<char *>(static_cast<void *>(&(iter->ID)))[k];
  //}
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mPosition_.x)))[k];
  }
  for (int k = 0; k < sizeof(float); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(t->mPosition_.y)))[k];
  }
  //std::cout << "LENGTH: " << i << " ---" << frameData.length() << std::endl;
  return data;
}


std::string ObjectSystem::GetTransformData(Object * iter)
{
  if (iter == nullptr || iter->dead) return "";
  TransformComponent * t = iter->GetComponent(TransformComponent);
  std::string data = "";
  int i = 0;
  ++i;
  for (int k = 0; k < sizeof(unsigned int); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(iter->ID)))[k];
    ++i;
  }
  //data += (iter->mVisible) ? '1' : '0';
  //if (!iter->mVisible) return data;
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

std::string ObjectSystem::GetTextureData(Object * iter)
{
  std::string data = "";
  if (iter == nullptr || iter->dead) return data;
  for (int k = 0; k < sizeof(unsigned int); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(iter->ID)))[k];
  }
  unsigned char len = gCore->GetSystem(GraphicsSystem)->mTextureMap_.find((static_cast<SpriteComponent *>(iter->GetComponent(SpriteComponent))->mTextureName))->second.index;
  data += len;
  return data;
}

std::string ObjectSystem::GetData(Object * iter)
{
  std::string data = "";
  if (iter == nullptr || iter->dead) return data;
  TransformComponent * t = iter->GetComponent(TransformComponent);
  int i = 0;
  ++i;
  for (int k = 0; k < sizeof(unsigned int); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(iter->ID)))[k];
    ++i;
  }
  unsigned char len = gCore->GetSystem(GraphicsSystem)->mTextureMap_.find((static_cast<SpriteComponent *>(iter->GetComponent(SpriteComponent))->mTextureName))->second.index;
  data += len;
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
  for (auto iter = bornObjects.begin(); iter != bornObjects.end(); ++iter){
    if (iter->second.second < 1)
    {
      //std::cout << "FUCK YOU BE ALIVE" << iter->first << std::endl;
      gCore->GetSystem(NetworkingSystem)->AddCommand('`', iter->first);
    }
    if (iter->second.second == 0){
      iter->second.second = 30;
    }
    iter->second.second -= 1;
  }
  for (auto iter = deadObjects.begin(); iter != deadObjects.end(); ++iter){
    if (iter->second.second < 1)
    {
      //std::cout << "FUCK YOU BE DIE" << iter->first << std::endl;
      gCore->GetSystem(NetworkingSystem)->AddCommand('%', iter->first);
    }
    if (iter->second.second == 0){
      iter->second.second = 30;
    }
    iter->second.second -= 1;
  }
  for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
  {
    auto node = iter->second.head;
    while (node){
      //node->value->hasChanged = true;
      
      if (node->value->dead){
        bool removable = true;
        for (auto compiter = node->value->mComponents.begin(); compiter != node->value->mComponents.end();){
          compiter->second->dead = true;
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
          deadObjects.insert({ node->value->ID, { 1, 1 } });
          --numObjects;

          for (auto eiter = bornObjects.begin(); eiter != bornObjects.end(); ++eiter){
            if (eiter->first == node->value->ID){
              bornObjects.erase(eiter);
              break;
            }
          }
          mObjectMap_.erase(mObjectMap_.find(node->value->ID));
          delete node->value;
          node = iter->second.Remove(node);
        }
        else{
          node->value->Update(dt);
          node = node->next;
        }
        continue;
      }
      if (node->value->hasChanged || node->value->age > rand() % 100 + 100){
        //frameData += GetData(node->value);

        auto * system = gCore->GetSystem(NetworkingSystem);
        system->AddCommand('#', node->value->ID);
        
        node->value->hasChanged = false;
      }
      else{
        //std::cout << node->value->age << std::endl;
        node->value->age += 1;
      }
      node->value->Update(dt);
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
  if (myCount == 0) myCount = 1;
  //obj->hasChanged = true;
  if (mObjects.find(obj->name) == mObjects.end())
  {
    mObjects.insert({ obj->name, List<Object*>() });
  }

  bornObjects.insert({obj->ID , {1,1} });
  mObjects[obj->name].AddToFront(obj);
  mObjectMap_.insert({ obj->ID, obj });
  auto * system = gCore->GetSystem(NetworkingSystem);
  system->AddCommand('`', obj->ID);
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
      break;
    }
  }
}
void ObjectSystem::RemoveBornObject(unsigned int ID)//FUCK YOU THIS DOESN'T DO WHAT YOU THINK IT DOES. IS FOR NATWURKN
{
  for (auto iter = bornObjects.begin(); iter != bornObjects.end(); ++iter){
    if (iter->first == ID){
      bornObjects.erase(iter);
      break;
    }
  }
}