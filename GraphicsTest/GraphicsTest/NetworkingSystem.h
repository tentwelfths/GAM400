#pragma once

#include "System.h"
#include "Standard.h"

#pragma comment(lib, "Ws2_32.lib")
class Object;
struct Connection
{
  SOCKET client;
  bool active;
  int initstep;
  Object * parent;
  unsigned char clientNumber;
  Connection(){
    client = INVALID_SOCKET;
    active = false;
    initstep = 0;
    clientNumber = 0;
  }
};

struct UDPConnection
{
  sockaddr addr;
  unsigned short frameCount;
  unsigned short lastFrameSeen;
  unsigned char clientNumber;
  bool update;
  Object * parent;
  int initstep;
  std::string unfinished;
  std::queue<std::string> commands;
  UDPConnection(sockaddr a) :addr(a), frameCount(0), lastFrameSeen(0), clientNumber(0), parent(nullptr), initstep(0){}
};

#define DEFAULT_PORT "27015"
#define NUMCLIENTS 100

class NetworkingComponent;

class NetworkingSystem : public System
{
public:
  NetworkingSystem();
  bool Initialize();
  void Update(double dt);
  void Shutdown();
  void RegisterComponent(NetworkingComponent * comp);
private:
  Connection sockets[NUMCLIENTS];
  WSADATA wsaData;
  struct addrinfo *result, *ptr, hints;
  SOCKET ListenSocket;
  unsigned char connectionCount;
  unsigned char clientCount;
  std::deque<int> openConnections;
  std::vector<NetworkingComponent *> mComponents_;
  std::vector<UDPConnection> connections;
  bool even;

};