#pragma once

enum class MessageType{COLLISIONSTARTED, CHANGELEDS, CAMERAMOVE};

class Object;

struct IMessage{
  IMessage(MessageType t) : type(t){}
  MessageType type;
};

struct CollisionStartedMessage : public IMessage{
  CollisionStartedMessage() : IMessage(MessageType::COLLISIONSTARTED){}
  Object * obj1,* obj2;
};

struct ChangeLEDSMessage : public IMessage{
  ChangeLEDSMessage() :IMessage(MessageType::CHANGELEDS){}
  bool state[10];
  char controllerNum;
};

struct CameraMoveMessage : public IMessage{
  CameraMoveMessage() :IMessage(MessageType::CAMERAMOVE){}
  unsigned int objID;
  char controllerNum;
};