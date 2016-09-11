#include "NetworkingSystem.h"
#include "Globals.h"
#include "Socket.h"
#include "InputSystem.h"
#include "Core.h"


NetworkingSystem::NetworkingSystem()
{
  mName_ = "NetworkingSystem";
}

void NetworkingSystem::RegisterComponent(NetworkingComponent * comp)
{
  mComponents_.push_back(comp);
}

bool NetworkingSystem::Initialize()
{
  //Init WSA
  int iResult;
  u_long iMode;
  connectionCount = 0;
  clientCount = 0;
  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return false;
  }
  result = NULL; 
  ptr = NULL;
  ListenSocket = INVALID_SOCKET;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve the local address and port to be used by the server
  iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
    printf("getaddrinfo failed: %d\n", iResult);
    WSACleanup();
    return false;
  }
  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (ListenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return false;
  }
  //bind socket to listen for new peeps
  iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR) {
    printf("bind failed with error: %d\n", WSAGetLastError());
    closesocket(ListenSocket);
    WSACleanup();
    return false;
  }
  freeaddrinfo(result);
  iResult = ioctlsocket(ListenSocket, FIONBIO, &(iMode));
  if (iResult != NO_ERROR)
  {
    printf("ioctlsocket failed with error: %ld\n", iResult);
    closesocket(ListenSocket);
    WSACleanup();
    return false;
  }
  if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
    printf("Listen failed with error: %ld\n", WSAGetLastError());
    closesocket(ListenSocket);
    WSACleanup();
    return false;
  }


  return true;
}

void NetworkingSystem::Update(double dt)
{
  int iResult;
  u_long iMode = 1;
  //grab any new devices that have connected
  while ((sockets[connectionCount].client = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
  {
    iResult = ioctlsocket(sockets[connectionCount].client, FIONBIO, &(iMode));
    if (iResult != NO_ERROR)
    {
      printf("CLIENT ioctlsocket failed with error: %ld\n", iResult);
      closesocket(sockets[connectionCount].client);
      continue;
    }
    sockets[connectionCount].active = true;
    sockets[connectionCount].initstep = 0;
    if (openConnections.size() == 0){
      sockets[connectionCount].clientNumber = clientCount;
    }
    else{
      sockets[connectionCount].clientNumber = openConnections.front();
      openConnections.pop_front();
    }
    ++clientCount;
    ++connectionCount;
    int start = connectionCount;
    while (sockets[connectionCount].active){
      if (connectionCount > NUMCLIENTS){
        connectionCount = 0;
      }
      ++connectionCount;
      if (connectionCount == start){
        std::cout << "WE HAVE ALL SOCKETS FULL. PANIC" << std::endl;
        break;
      }
    }
    if (connectionCount > NUMCLIENTS){
      connectionCount = 0;
    }
    if (connectionCount == start){
      break;
    }
  }
  //grab some packets
  char buf[256];
  for (int i = 0; i < NUMCLIENTS; ++i){
    if (!sockets[i].active)continue;
    SecureZeroMemory(buf, 256);
    int result = recv(sockets[i].client, buf, 255, 0);
    if (result == SOCKET_ERROR || result == 0){
      if (WSAGetLastError() != WSAEWOULDBLOCK || result == 0){
        sockets[i].active = false;
        closesocket(sockets[i].client);
        std::cout << "We lost a dude" << std::endl;
        openConnections.push_back(sockets[i].clientNumber);
        std::sort(openConnections.begin(), openConnections.end());
        --clientCount;
        continue;
      }
      continue;
    }
    if (sockets[i].initstep == 0){
      std::string str = "Client number: " + std::to_string(sockets[i].clientNumber);
      result = send(sockets[i].client, str.c_str(), strlen(str.c_str()), 0);
    }
    std::cout << "Got some info from client #" << i << " ---> " << buf << std::endl;
    InputSystem * input = gCore->GetSystem(InputSystem);
    for (int pos = 0; pos < result; ++pos)
    {
      int key = buf[pos++];
      bool val = (pos == 0) ? false : true;
      input->setKey(key,val);
    }
  }
}

void NetworkingSystem::Shutdown()
{
  //close all sockets
  for (int i = 0; i < NUMCLIENTS; ++i){
    if (!sockets[i].active)continue;
    closesocket(sockets[i].client);
  }
  closesocket(ListenSocket);
  //shutdown wsa
  WSACleanup();
}
