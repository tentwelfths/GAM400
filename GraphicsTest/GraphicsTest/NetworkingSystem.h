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
  float x, y;
  unsigned char clientNumber;
  Connection(){
    client = INVALID_SOCKET;
    active = false;
    initstep = 0;
    clientNumber = 0;
    x = 0; y = 0;
  }
};

struct Command
{
  char comType;
  unsigned int ID;
  char data[64];
  Command() {}
  Command(char c, unsigned int I, char d[64]) :comType(c), ID(I){ if (d == nullptr)return; for (unsigned i = 0; i < 64; ++i)data[i] = d[i]; }
};

struct UDPConnection
{
  sockaddr_in addr;
  unsigned short frameCount;
  unsigned short lastFrameSeen;
  unsigned char clientNumber;
  bool update;
  char playerNum;
  int initstep;
  float x, y;
  std::string unfinished;
  std::vector<unsigned int> unloaded;
  std::queue<std::string> commandsRec;
  std::queue<Command> commandsSend;
  UDPConnection(sockaddr_in a) :addr(a), frameCount(0), lastFrameSeen(0), clientNumber(0), initstep(0), x(0.f), y(0.f){}
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
  void AddCommand(char type, unsigned int ID, char data[64] = {0});
  void AddCommand(int connectionNumber, char type, unsigned int ID, char data[64] = { 0 });
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
  std::string ConstructCommand(char type, unsigned int ID, char data[64], int num);
};