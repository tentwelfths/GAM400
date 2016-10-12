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
  sockaddr_in addr;
  unsigned short frameCount;
  unsigned short lastFrameSeen;
  unsigned char clientNumber;
  bool update;
  Object * parent;
  int initstep;
  std::string unfinished;
  std::queue<std::string> commands;
  UDPConnection(sockaddr_in a) :addr(a), frameCount(0), lastFrameSeen(0), clientNumber(0), parent(nullptr), initstep(0){}
};

struct Command
{
  std::string command;
  char sendCount;
};

#define DEFAULT_PORT 27015
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
  void AddCommand(char type, unsigned int ID);
private:
  Connection sockets[NUMCLIENTS];
  WSADATA wsaData;
  //struct addrinfo *ptr, hints;
  sockaddr_in server;
  SOCKET ListenSocket;
  unsigned char connectionCount;
  unsigned char clientCount;
  std::deque<int> openConnections;
  std::vector<NetworkingComponent *> mComponents_;
  std::vector<UDPConnection> connections;
  bool even;
  std::vector<Command> mCommands;

};