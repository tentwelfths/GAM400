#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include "glm\detail\type_vec.hpp"

class TransformComponent : public IComponent
{
public:
  TransformComponent();
  bool Initialize();
  void Update(double dt);
  void Shutdown();

  glm::vec3 mPosition_;
  glm::vec3 mScale_;
  glm::vec3 mRotation_;

public:
  void mPosition(glm::vec3 pos);//
  void mPosition(float x, float y, float z);// { mPosition_ = glm::vec3(x, y, z); }
  void mPositionX(float x);//{ mPosition_.x = x; }
  void mPositionY(float y);//{ mPosition_.y = y; }
  void mPositionZ(float z);//{ mPosition_.z = z; }
  glm::vec3 mPosition();//{ return mPosition_; }
  float mPositionX();//{ return mPosition_.x; }
  float mPositionY();//{ return mPosition_.y; }
  float mPositionZ();//{ return mPosition_.z; }

  void      mScale(glm::vec3 pos);// { mScale_ = pos; }
  void      mScale(float x, float y, float z);// { mScale_ = glm::vec3(x, y, z); }
  void      mScaleX(float x);//{ mScale_.x = x; }
  void      mScaleY(float y);//{ mScale_.y = y; }
  void      mScaleZ(float z);//{ mScale_.z = z; }
  glm::vec3 mScale();//{ return mScale_; }
  float     mScaleX();//{ return mScale_.x; }
  float     mScaleY();//{ return mScale_.y; }
  float     mScaleZ();//{ return mScale_.z; }

  void      mRotation(glm::vec3 pos)             ;//{ mRotation_ = pos; }
  void      mRotation(float x, float y, float z) ;//{ mRotation_ = glm::vec3(x, y, z); }
  void      mRotationX(float x)                  ;//{ mRotation_.x = x; }
  void      mRotationY(float y)                  ;//{ mRotation_.y = y; }
  void      mRotationZ(float z)                  ;//{ mRotation_.z = z; }
  glm::vec3 mRotation()                   ;//{ return mRotation_; }
  float     mRotationX()                  ;//{ return mRotation_.x; }
  float     mRotationY()                  ;//{ return mRotation_.y; }
  float     mRotationZ()                  ;//{ return mRotation_.z; }

};