#include "NetworkingSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "Globals.h"
#include "Socket.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "ObjectSystem.h"
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
#define BUFLEN 512
bool NetworkingSystem::Initialize()
{


  //Initialise winsock
  printf("\nInitialising Winsock...");
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
  {
    printf("Failed. Error Code : %d", WSAGetLastError());
    exit(EXIT_FAILURE);
  }
  printf("Initialised.\n");

  //Create a socket
  if ((ListenSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
  {
    printf("Could not create socket : %d", WSAGetLastError());
  }
  printf("Socket created.\n");

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(DEFAULT_PORT);

  //Bind
  if (bind(ListenSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
  {
    printf("Bind failed with error code : %d", WSAGetLastError());
    exit(EXIT_FAILURE);
  }
  puts("Bind done");
  int iResult;
  u_long iMode = 1;
  iResult = ioctlsocket(ListenSocket, FIONBIO, &(iMode));
  if (iResult != NO_ERROR)
  {
    printf("ioctlsocket failed with error: %ld\n", iResult);
    closesocket(ListenSocket);
    WSACleanup();
    return false;
  }
  //keep listening for data
  //while (1)
  //{
  //  printf("Waiting for data...");
  //  fflush(stdout);
  //
  //  //clear the buffer by filling null, it might have previously received data
  //  memset(buf, '\0', BUFLEN);
  //
  //  //try to receive some data, this is a blocking call
  //  if ((recv_len = recvfrom(ListenSocket, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
  //  {
  //    printf("recvfrom() failed with error code : %d", WSAGetLastError());
  //    exit(EXIT_FAILURE);
  //  }
  //
  //  //print details of the client/peer and the data received
  //  printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
  //  printf("Data: %s\n", buf);
  //
  //  //now reply the client with the same data
  //  if (sendto(ListenSocket, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
  //  {
  //    printf("sendto() failed with error code : %d", WSAGetLastError());
  //    exit(EXIT_FAILURE);
  //  }
  //}
  /*
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

  ListenSocket = INVALID_SOCKET;

  // Resolve the local address and port to be used by the server
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(DEFAULT_PORT);

  ListenSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (ListenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    WSACleanup();
    return false;
  }
  //bind socket to listen for new peeps
  iResult = bind(ListenSocket, (struct sockaddr *)&server, sizeof(server));
  if (iResult == SOCKET_ERROR) {
    printf("bind failed with error: %d\n", WSAGetLastError());
    closesocket(ListenSocket);
    WSACleanup();
    return false;
  }
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
  */

  return true;
}

void NetworkingSystem::Update(double dt)
{
  int iResult;
  u_long iMode = 1;
  char buf[256] = { 0 };
  InputSystem * input = gCore->GetSystem(InputSystem);
  std::string frameData = gCore->GetSystem(ObjectSystem)->frameData;
  sockaddr_in addr;
  int fromlen = sizeof(sockaddr_in);
  while ((iResult = recvfrom(ListenSocket, buf, 255, 0, (sockaddr*)(&addr), &fromlen)) && iResult > 0)
  {

    //std::cout << "We got something capn" << std::endl;
    var same = false;
    var index = 0;
    for (unsigned i = 0; i < connections.size(); ++i)
    {
      if (connections[i].addr.sin_family == addr.sin_family)
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
    std::cout << "Got " << iResult << "bytes" << std::endl;
    std::string str = "";
    for (var i = 0; i < iResult; ++i)
    {
      //std::cout << i <<" : " << buf[i]<<std::endl;
      str += buf[i];
      //if (buf[i] == '!'){
      //  std::cout << "COMMAND DONE RECORDING FUCK" << std::endl;
      //  connections[index].unfinished += buf[i];
      //  connections[index].commands.push(connections[index].unfinished);
      //  connections[index].unfinished = "";
      //}
      //else
      //{
      //  connections[index].unfinished += buf[i];
      //}
    }
    //std::cout<<std::endl;
    connections[index].commands.push(str);
    memset(buf, 0, 256);
  }
  //if (iResult  == SOCKET_ERROR)
  //{
  //  printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
  //  //exit(EXIT_FAILURE);
  //}
  //std::cout << connections.size() << std::endl;
  even = !even;
  for (unsigned i = 0; i < connections.size(); ++i)
  {
    while (!connections[i].commands.empty())
    {
      std::string command = connections[i].commands.front(); connections[i].commands.pop();
      std::cout <<command.length() <<"    " << command << std::endl;
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
      else if (command[0] == '%')//ACKing DEATH
      {
        int pos = 1;
        int ded = *static_cast<const unsigned int*>(static_cast<const void *>(&(command.c_str()[pos])));
        auto * objSys = gCore->GetSystem(ObjectSystem);
        objSys->RemoveDeadObject(ded);
      }
      else if (command[0] == '~')//input
      {
        int pos = 1;
        std::cout << "GOT INPUT" << std::endl;
        unsigned short x, y;
        bool button;
        x = *static_cast<const unsigned short*>(static_cast<const void *>(&(command.c_str()[pos])));
        pos += sizeof(unsigned short);
        y = *static_cast<const unsigned short*>(static_cast<const void *>(&(command.c_str()[pos])));
        pos += sizeof(unsigned short);
        //std::cout << "THE COMMAND SAYETH" << command[pos] << std::endl;
        button = (command[pos] == '1');
        std::vector<int> buttons;
        buttons.push_back(0); 
        std::vector<bool> state;
        state.push_back(button);
        input->updateController(0, buttons, state, (x - 512) / 512.f, (y - 512) / 512.f);
        //auto * obj = gCore->GetSystem(ObjectSystem)->GetFirstItemByName("Fuccboi");
        //auto * trans = obj->GetComponent(TransformComponent);
        //trans->mPositionX(((x - 512) / 512.0f) * 4.f);
        //trans->mPositionY(((y - 512) / 512.0f) * 4.f);

        //unsigned short frame = *static_cast<const unsigned short *>(static_cast<const void *>(&(command.c_str()[pos])));
        //pos += sizeof(unsigned short);

        //int key = command[pos];
        //pos += 2;
        //bool val = (command[pos] == '1') ? true : false;
        //if (frame > connections[i].lastFrameSeen)
        //{
        //  connections[i].lastFrameSeen = frame;
        //  input->setRaspKey(key, val, connections[i].clientNumber);
        //}
      }
    }
    if (((even && i % 2 == 0) || (!even && i % 2 == 1)) && frameData != ""){
      //int b = send(sockets[i].client, frameData.c_str(), frameData.length(), 0);
      std::string toSend = ""; 
      
      toSend += static_cast<char *>(static_cast<void*>(&connections[i].frameCount))[0];
      toSend += static_cast<char *>(static_cast<void*>(&connections[i].frameCount))[1];
      //toSend += frameData;
      for (unsigned k = 0; k < mCommands.size() && toSend.length() + mCommands[i].command.length() < 1020; ++k){
        toSend += '!' + mCommands[k].command;
        ++mCommands[k].sendCount;
        if (mCommands[k].sendCount > 1){
          mCommands.erase(mCommands.begin() + k);
          --k;
        }
      }
      ++connections[i].frameCount;
      int b = sendto(ListenSocket, toSend.c_str(), toSend.length(), 0, (sockaddr*)&connections[i].addr, sizeof(sockaddr_in));
      //std::cout << "Send: " << toSend << std::endl;
      //std::cout << "Sent " << b << " bytes." << std::endl;
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

void NetworkingSystem::AddCommand(char type, unsigned int ID)
{
  switch (type){
  case '`': // Object created. 
  {
    mCommands.push_back({ std::string(type + gCore->GetSystem(ObjectSystem)->GetData(ID)), 0 });
  }
    break;

  case '%': //Object died
  {
    std::string str = "" + type;
    for (int k = 0; k < sizeof(unsigned int); ++k)
    {
      str += static_cast<char *>(static_cast<void *>(&(ID)))[k];
    }
    mCommands.push_back({ str, 0 });
  }
    break;

  case '#': //Object moved
  {
    mCommands.push_back({ std::string(type + gCore->GetSystem(ObjectSystem)->GetTransformData(ID)), 0 });
  }
    break;

  case '$': //Object texture changed
  {
    mCommands.push_back({ std::string(type + gCore->GetSystem(ObjectSystem)->GetTextureData(ID)), 0 });
  }
    break;

  case '^': //Update led bar graph
  {

  }
    break;

  case '&': //play sound effect
  {

  }
    break;
  }
}
