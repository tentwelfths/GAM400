#pragma once

enum class MessageType{COLLISIONSTARTED, COLLISIONENDED, CHANGELEDS, CAMERAMOVE, CHANGETEXTURE};

class Object;

struct IMessage{
  IMessage(MessageType t) : type(t){}
  MessageType type;
  char data[64];
};

struct CollisionStartedMessage{
  CollisionStartedMessage(){}
  Object * obj1,* obj2;
};

struct CollisionEndedMessage{
  CollisionEndedMessage(){}
  Object * obj1, *obj2;
};

struct ChangeLEDSMessage{
  ChangeLEDSMessage(){}
  bool state[10];
  char controllerNum;
};

struct CameraMoveMessage {
  CameraMoveMessage(){}
  unsigned int objID;
  char controllerNum;
};

struct ChangeTextureMessage {
  ChangeTextureMessage(){}
  unsigned int objID;
};