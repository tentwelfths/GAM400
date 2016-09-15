#include "NetworkingSystem.h"
#include "Globals.h"
#include "Socket.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "Core.h"
#define var auto

NetworkingSystem::NetworkingSystem()
{
  mName_ = "NetworkingSystem";
  even = false;
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
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
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
  //if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
  //  printf("Listen failed with error: %ld\n", WSAGetLastError());
  //  closesocket(ListenSocket);
  //  WSACleanup();
  //  return false;
  //}


  return true;
}

void NetworkingSystem::Update(double dt)
{
  int iResult;
  u_long iMode = 1;
  char buf[256] = { 0 };
  InputSystem * input = gCore->GetSystem(InputSystem);
  std::string frameData = gCore->GetSystem(GraphicsSystem)->frameData;
  sockaddr addr;
  int fromlen;
  while ((iResult = recvfrom(ListenSocket, buf, 255, 0, (&addr), &fromlen)) && iResult > 0)
  {
    std::cout << "We got something capn" << std::endl;
    var same = false;
    var index = 0;
    for (int i = 0; i < connections.size(); ++i)
    {
      if (connections[i].addr.sa_family == addr.sa_family)
      {
        if (((sockaddr_in*)&connections[i].addr)->sin_addr.s_addr == ((sockaddr_in*)&addr)->sin_addr.s_addr)
        {
          same = true;
          index = i;
          break;
        }
      }
    }
    if (!same)
    {
      std::cout << "NEW DUDE" << std::endl;
      index = connections.size();
      connections.push_back(UDPConnection(addr));
    }

    for (var i = 0; i < iResult; ++i)
    {
      if (buf[i] == '~'){
        connections[index].commands.push(connections[index].unfinished);
        connections[index].unfinished = "";
      }
      else
      {
        connections[index].unfinished += buf[i];
      }
    }
    memset(buf, 0, 256);
  }

  for (int i = 0; i < connections.size(); ++i)
  {
    while (!connections[i].commands.empty())
    {
      std::string command = connections[i].commands.front(); connections[i].commands.pop();
      if (command.find("HELLO") != std::string::npos)
      {
        connections[i].update = (i % 2) ? true :false;
        if (openConnections.size() == 0){
          connections[i].clientNumber = clientCount;
        }
        else{
          connections[i].clientNumber = openConnections.front();
          openConnections.pop_front();
        }
        ++clientCount;
        ++connectionCount;
      }
      else if (command.find("BYE") != std::string::npos)
      {
        std::cout << "We lost a dude" << std::endl;
        openConnections.push_back(connections[i].clientNumber);
        std::sort(openConnections.begin(), openConnections.end());
        connections.erase(connections.begin() + i);
        --clientCount;
        --i;
        break;
      }
      else if (command[0] == '~')//keyboard
      {
        int pos = 1;
        unsigned short frame = *static_cast<const unsigned short *>(static_cast<const void *>(&(command.c_str()[pos])));
        pos += sizeof(unsigned short);
        int key = command[pos];
        bool val = (command[pos+1] == '1') ? true : false;
        if (frame > connections[i].lastFrameSeen)
        {
          connections[i].lastFrameSeen = frame;
          input->setRaspKey(key, val, connections[i].clientNumber);
        }
      }
    }
    if ((even && i % 2 == 0) || (!even && i % 2 == 1)){
      //int b = send(sockets[i].client, frameData.c_str(), frameData.length(), 0);
      std::string toSend = "`"; 
      toSend += static_cast<char *>(static_cast<void*>(&connections[i].frameCount)) + frameData + "!";
      ++connections[i].frameCount;
      int b = sendto(ListenSocket, toSend.c_str(), toSend.length(), 0, &connections[i].addr, sizeof(connections[i].addr));
      std::cout << "Send: " << toSend << std::endl;
      std::cout << "Sent " << b << " bytes." << std::endl;
    }
  }
  //grab any new devices that have connected
  //while ((sockets[connectionCount].client = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
  //{
  //  iResult = ioctlsocket(sockets[connectionCount].client, FIONBIO, &(iMode));
  //  if (iResult != NO_ERROR)
  //  {
  //    printf("CLIENT ioctlsocket failed with error: %ld\n", iResult);
  //    closesocket(sockets[connectionCount].client);
  //    continue;
  //  }
  //  sockets[connectionCount].active = true;
  //  sockets[connectionCount].initstep = 0;
  //  if (openConnections.size() == 0){
  //    sockets[connectionCount].clientNumber = clientCount;
  //  }
  //  else{
  //    sockets[connectionCount].clientNumber = openConnections.front();
  //    openConnections.pop_front();
  //  }
  //  ++clientCount;
  //  ++connectionCount;
  //  int start = connectionCount;
  //  while (sockets[connectionCount].active){
  //    if (connectionCount > NUMCLIENTS){
  //      connectionCount = 0;
  //    }
  //    ++connectionCount;
  //    if (connectionCount == start){
  //      std::cout << "WE HAVE ALL SOCKETS FULL. PANIC" << std::endl;
  //      break;
  //    }
  //  }
  //  if (connectionCount > NUMCLIENTS){
  //    connectionCount = 0;
  //  }
  //  if (connectionCount == start){
  //    break;
  //  }
  //}
  ////grab some packets
  //char buf[256];
  //even = !even;
  //
  //InputSystem * input = gCore->GetSystem(InputSystem);
  //std::string frameData = gCore->GetSystem(GraphicsSystem)->frameData;
  //for (int i = 0; i < NUMCLIENTS; ++i){
  //  if (!sockets[i].active)continue;
  //  SecureZeroMemory(buf, 256);
  //  int result = recv(sockets[i].client, buf, 255, 0);
  //  if (result == SOCKET_ERROR || result == 0){
  //    if (WSAGetLastError() != WSAEWOULDBLOCK || result == 0){
  //      sockets[i].active = false;
  //      closesocket(sockets[i].client);
  //      std::cout << "We lost a dude" << std::endl;
  //      openConnections.push_back(sockets[i].clientNumber);
  //      std::sort(openConnections.begin(), openConnections.end());
  //      --clientCount;
  //      continue;
  //    }
  //  }
  //  if (sockets[i].initstep == 0){
  //    std::string str = "~" + std::to_string(sockets[i].clientNumber) + "~";
  //    sockets[i].initstep = 5;
  //    result = send(sockets[i].client, str.c_str(), strlen(str.c_str()), 0);
  //  }
  //  if (result > 0){
  //    std::cout << "Got " << result << " bytes from client #" << i << " ---> " << buf << std::endl;
  //  }
  //  for (int pos = 0; pos < result; ++pos)
  //  {
  //    int key = buf[pos++];
  //    bool val = (buf[pos] == '1') ? true : false;
  //    input->setRaspKey(key,val);
  //  }
  //  if ((even && i % 2 == 0) || (!even && i % 2 == 1)){
  //    int b = send(sockets[i].client, frameData.c_str(), frameData.length(), 0);
  //    std::cout << "Send: " << frameData << std::endl;
  //    std::cout << "Sent " << b << " bytes." << std::endl;
  //  }
  // 
  //}
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
