#include "Object.h"
#include "ObjectSystem.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

ObjectSystem::ObjectSystem()
{
  mName_ = "ObjectSystem";
  myCount = 0;
}

void     ObjectSystem::RegisterComponent(IComponent*)
{

}

bool     ObjectSystem::Initialize()
{
  return true;
}

std::string ObjectSystem::GetData(Object * iter)
{
  TransformComponent * t = iter->GetComponent(TransformComponent);
  std::string data = "";
  int i = 0;
  ++i;
  for (int k = 0; k < sizeof(unsigned int); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(iter->ID)))[k];
    ++i;
  }
  for (int k = 0; k < sizeof(GLuint); ++k)
  {
    data += static_cast<char *>(static_cast<void *>(&(static_cast<SpriteComponent *>(iter->GetComponent(SpriteComponent))->mTexture_)))[k];
    ++i;
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
  for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
  {
    auto node = iter->second.head;
    while (node){
      //node->value->hasChanged = true;
      if (node->value->hasChanged || node->value->age > 10){
        frameData += GetData(node->value);
        node->value->age = 0;
        node->value->hasChanged = false;
      }
      else{
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
}

Object * ObjectSystem::GetFirstItemByName(std::string name)
{
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