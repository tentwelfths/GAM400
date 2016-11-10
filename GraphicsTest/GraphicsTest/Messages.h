#pragma once

enum class MessageType{COLLISIONSTARTED, COLLISIONENDED, CHANGELEDS, CAMERAMOVE, CHANGETEXTURE, PLAYSOUND, PLAY3DSOUND};

class Object;
class TransformComponent;

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

struct PlaySoundMessage {
  PlaySoundMessage(){}
  char name[48];
};

struct Play3DSoundMessage {
  Play3DSoundMessage(){}
  TransformComponent * source,* listener;
  char name[48];
};