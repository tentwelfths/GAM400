#pragma once

enum class MessageType{COLLISIONSTARTED};

class Object;

struct IMessage{
  IMessage(MessageType t) : type(t){}
  MessageType type;
};

struct CollisionStartedMessage : public IMessage{
  CollisionStartedMessage() : IMessage(MessageType::COLLISIONSTARTED){}
  Object * obj1,* obj2;
};