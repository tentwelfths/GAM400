#include "NetworkingSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "Globals.h"
#include "Socket.h"
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "ObjectSystem.h"
#include "Core.h"
#include "Messages.h"
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
  return true;
}
#include <bitset>
void NetworkingSystem::Update(double dt)
{
  int iResult;
  u_long iMode = 1;
  char buf[256] = { 0 };
  InputSystem * input = gCore->GetSystem(InputSystem);
  auto * objsys = gCore->GetSystem(ObjectSystem);
  sockaddr_in addr;
  int fromlen = sizeof(sockaddr_in);
  int c = 0;

  for (auto iter : mMessages_){
    switch (iter.type){
    case MessageType::CHANGELEDS:{
      auto * msg = reinterpret_cast<ChangeLEDSMessage *>(iter.data);
      char d[8] = { 0 };

      //for (int i = 0; i < 10; ++i) d[(i > 7) ? 1 : 0] |= ((msg->state[i]) ? 1 : 0) << ((i % 8));
      //for (int i = 0; i < 2; ++i){
      //  std::cout << std::bitset<8>(d[i]);
      //}
      //std::cout << std::endl;
      std::bitset<8> d1;
      std::bitset<8> d2;
      for (int i = 0; i < 8; ++i){
        d1[i] = msg->state[i];
      }
      for (int i = 0; i < 2; ++i){
        d2[i] = msg->state[8 + i];
      }
      d[0] = (char)d1.to_ulong();
      d[1] = (char)d2.to_ulong();
      for (unsigned i = 0; i < connections.size(); ++i){
        if (connections[i].playerNum == msg->controllerNum){
          AddCommand(i, '^', 0, d);
        }
      }
      break;
    }
    case MessageType::CAMERAMOVE:{
      auto * msg = reinterpret_cast<CameraMoveMessage *>(iter.data);
      for (unsigned i = 0; i < connections.size(); ++i){
        if (connections[i].playerNum == msg->controllerNum){
          auto obj = gCore->GetSystem(ObjectSystem)->mObjectMap_.find(msg->objID);
          if (obj != gCore->GetSystem(ObjectSystem)->mObjectMap_.end()){
            if (obj->second){
              auto *trans = obj->second->GetComponent(TransformComponent);
              if (trans){
                AddCommand(i, '(', msg->objID);

                connections[i].x = obj->second->GetComponent(TransformComponent)->mPositionX();
                connections[i].y = obj->second->GetComponent(TransformComponent)->mPositionY();
              }
            }
          }
        }
      }
      
      break;
    }
    case MessageType::SPRITECHANGED:{
      auto * msg = reinterpret_cast<SpriteChangeMessage *>(iter.data);
      AddCommand('$', msg->objID);
      break;
    }
    case MessageType::PLAYSOUND:{
      //auto * msg = reinterpret_cast<PlaySoundMessage *>(iter.data);
      AddCommand('&', 0, iter.data);
      break;
    }
    case MessageType::PLAY3DSOUND:{
      auto * msg = reinterpret_cast<Play3DSoundMessage *>(iter.data);
      std::cout << msg->name << std::endl;
      AddCommand('*', 0, iter.data);
      break;
    }
    }
  }
  mMessages_.clear();
  while ((iResult = recvfrom(ListenSocket, buf, 255, 0, (sockaddr*)(&addr), &fromlen)) && iResult > 0)
  {

    //std::cout << "We got something capn" << std::endl;
    std::cout << buf << std::endl;
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
      //std::cout << "NEW DUDE" << std::endl;
      index = connections.size();
      connections.push_back(UDPConnection(addr));
    }
    //std::cout << "Got " << iResult << "bytes" << std::endl;
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
    connections[index].commandsRec.push(str);
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
    while (!connections[i].commandsRec.empty())
    {
      std::string command = connections[i].commandsRec.front(); connections[i].commandsRec.pop();
      //std::cout <<command.length() <<"    " << command << std::endl;
      if (command[0] == '+')
      {
        std::cout << "GOT A DUDE" << std::endl;
        connections[i].update = (i % 2) ? true :false;
        if (openConnections.size() == 0){
          connections[i].clientNumber = clientCount;
        }
        else{
          connections[i].clientNumber = openConnections.front();
          openConnections.pop_front();
        }
        connections[i].playerNum = command[1];
        std::cout << "Got player num" << (int)connections[i].playerNum << std::endl;
        connections[i].initstep = 0;
        ++clientCount;
        ++connectionCount;
      }
      else if (command[0] == '-')
      {
        //std::cout << "We lost a dude" << std::endl;
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
        std::cout << "DEATH ACKD" << std::endl;
        unsigned int ded = *static_cast<const unsigned int*>(static_cast<const void *>(&(command.c_str()[pos])));
        auto * objSys = gCore->GetSystem(ObjectSystem);
        objSys->RemoveDeadObject(ded);
      }
      else if (command[0] == '`')//ACKing BORN
      {
        int pos = 1;
        std::cout << "BORN ACKD" << std::endl;
        int ded = *static_cast<const unsigned int*>(static_cast<const void *>(&(command.c_str()[pos])));
        auto * objSys = gCore->GetSystem(ObjectSystem);
        objSys->RemoveBornObject(ded);
      }
      else if (command[0] == 'L')//ACKing LOAD
      {
        int pos = 1;
        std::cout << "LOAD ACKD" << std::endl;   
        
        //if (command.substr(1) == gCore->GetCurrentLevelName()){
          connections[i].initstep = 301;
        //}
      }
      else if (command[0] == 'M')//Load Done
      {
        int pos = 1;
        std::cout << "Done loading" << std::endl;
        connections[i].initstep = -1;
      }
      else if (command[0] == '~')//input
      {
        int pos = 1;
        //std::cout << "GOT INPUT" << std::endl;
        unsigned short x1, y1, x2, y2;
        bool button1, button2;
        char knobDelta;
        x1 = *static_cast<const unsigned short*>(static_cast<const void *>(&(command.c_str()[pos])));
        pos += sizeof(unsigned short);
        y1 = *static_cast<const unsigned short*>(static_cast<const void *>(&(command.c_str()[pos])));
        pos += sizeof(unsigned short);
        x2 = *static_cast<const unsigned short*>(static_cast<const void *>(&(command.c_str()[pos])));
        pos += sizeof(unsigned short);
        y2 = *static_cast<const unsigned short*>(static_cast<const void *>(&(command.c_str()[pos])));
        pos += sizeof(unsigned short);
        //std::cout << "THE COMMAND SAYETH" << command[pos] << std::endl;
        button1 = (command[pos] == '1');
        ++pos;
        button2 = (command[pos] == '1');
        ++pos;
        if (connections[i].playerNum == 0)//coneman
        {
          knobDelta = command[pos];
          ++pos;
          std::vector<int> buttons;
          buttons.push_back(0);
          buttons.push_back(1);
          std::vector<bool> state;
          state.push_back(button1);
          state.push_back(button2);
          if (knobDelta != 0)
          std::cout << (int)knobDelta << std::endl;
          input->updateController(connections[i].playerNum, buttons, state, (x1 - 512) / 512.f, (y1 - 512) / 512.f, (x2 - 512) / 512.f, (y2 - 512) / 512.f, knobDelta);
        }
        else if (connections[i].playerNum == 1){
          std::vector<int> buttons;
          buttons.push_back(0);
          buttons.push_back(1);
          buttons.push_back(2);
          buttons.push_back(3);
          buttons.push_back(4);
          std::vector<bool> state;
          state.push_back(button1);
          state.push_back(button2);
          state.push_back((command[pos++] == '1'));
          state.push_back((command[pos++] == '1'));
          state.push_back((command[pos++] == '1'));
          input->updateController(connections[i].playerNum, buttons, state, (x1 - 512) / 512.f, (y1 - 512) / 512.f, (x2 - 512) / 512.f, (y2 - 512) / 512.f);
        }
        else{
          std::cout << "Updating controller" << (int)connections[i].playerNum << std::endl;
          std::vector<int> buttons;
          buttons.push_back(0);
          buttons.push_back(1);
          std::vector<bool> state;
          state.push_back(button1);
          state.push_back(button2);
          input->updateController(connections[i].playerNum, buttons, state, (x1 - 512) / 512.f, (y1 - 512) / 512.f, (x2 - 512) / 512.f, (y2 - 512) / 512.f);
        }
      }
    }
    //if (/*((even && i % 2 == 0) || (!even && i % 2 == 1)) && */connections[i].commandsSend.size() > 0)
    {
      //int b = send(sockets[i].client, frameData.c_str(), frameData.length(), 0);
      std::string toSend = "";
      if (connections[i].initstep == 0){
        char data[64];
        strcpy(data, gCore->GetCurrentLevelName().c_str());
        std::cout << "SENDING THE L " << data << std::endl;
        AddCommand(i, 'L', 0, data);
      }

      //toSend += static_cast<char *>(static_cast<void*>(&connections[i].frameCount))[0];
      //toSend += static_cast<char *>(static_cast<void*>(&connections[i].frameCount))[1];
      //toSend += frameData;
      if (connections[i].initstep > 0) ++connections[i].initstep;
      std::cout << connections[i].initstep<<std::endl;
      if (connections[i].initstep > 300){
        std::cout << "SENDING THE M"<< std::endl;
        AddCommand('M', i);
        connections[i].initstep = 1;
      }
      for (unsigned k = 0; connections[i].commandsSend.empty() == false && k < 30; ++k){
        std::string temp = ConstructCommand(connections[i].commandsSend.front().comType, connections[i].commandsSend.front().ID, connections[i].commandsSend.front().data, connections[i].playerNum);
        if (toSend.length() + temp.length() > 1023){
          break;
        }
        if (temp[0] == '#'){
          //std::cout << "SNEDING MOVE" << std::endl;
        }
        connections[i].commandsSend.pop();
        toSend += temp;
        //toSend += '\0';
      }
      ++connections[i].frameCount;
      if (toSend.length() > 0){
        //std::cout << "Sending " << toSend.length() << std::endl;
        int b = sendto(ListenSocket, toSend.c_str(), toSend.length(), 0, (sockaddr*)&connections[i].addr, sizeof(sockaddr_in));
      }
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

void NetworkingSystem::AddCommand(char com, unsigned int ID, char data[64])
{
  for (unsigned i = 0; i < connections.size(); ++i){
    if (com == '#'){
      auto obj = gCore->GetSystem(ObjectSystem)->mObjectMap_.find(ID);
      if (obj == gCore->GetSystem(ObjectSystem)->mObjectMap_.end() || obj->second == nullptr) return;
      auto * trans = obj->second->GetComponent(TransformComponent);
      if (!(trans->mPositionX() >= connections[i].x - trans->mScaleX() - 15 &&
        trans->mPositionY() >= connections[i].y - trans->mScaleY() - 15 &&
        trans->mPositionX() <= connections[i].x + trans->mScaleX() + 15 &&
        trans->mPositionY() <= connections[i].y + trans->mScaleY() + 15)){
        continue;
      }
    }
    connections[i].commandsSend.push({ com, ID, data});
  }
}

void NetworkingSystem::AddCommand(int connectionNumber, char com, unsigned int ID, char data[64])
{
  connections[connectionNumber].commandsSend.push({ com, ID, data});
}

std::string NetworkingSystem::ConstructCommand(char com, unsigned int ID, char data[64], int num)
{
  std::string temp;
  switch (com){
  case '`': // Object created. 
  {
    std::string data = gCore->GetSystem(ObjectSystem)->GetData(ID, num);

    if (data == "") break;
    temp = "`";
    for (unsigned i = 0; i < data.length(); ++i){
      temp += data[i];
    }
  }
    break;

  case 'L':
  {
    if (data == "") break;
    temp = "L";
    std::string name = data;
    temp += (char)(name.length());
    for (unsigned i = 0; i < name.length(); ++i){
      temp += name[i];
    }
  }
    break;
  case 'M':
  {
    temp = "M";
  }
    break;

  case '%': //Object died
  {
    temp = "%";
    if (ID == 0){
      int b = 10;
    }
    for (int k = 0; k < sizeof(unsigned int); ++k)
    {
      temp += static_cast<char *>(static_cast<void *>(&(ID)))[k];
    }
  }
    break;

  case '#': //Object moved
  {
    //std::cout << "MOVE MESSAGE" << gCore->GetSystem(ObjectSystem)->mObjectMap_[ID]->name << std::endl;
    std::string data = gCore->GetSystem(ObjectSystem)->GetTransformData(ID, num);
    if (data == "") break;
    temp = "#";
    for (unsigned i = 0; i < data.length(); ++i){
      temp += data[i];
    }
  }
    break;

  case '$': //Object texture changed
  {
    std::string data = gCore->GetSystem(ObjectSystem)->GetTextureData(ID);
    if (data == "") break;
    temp = "$";
    for (unsigned i = 0; i < data.length(); ++i){
      temp += data[i];
    }
  }
    break;

  case '(': //Update camera position
  {
    std::string data = gCore->GetSystem(ObjectSystem)->GetCameraData(ID);
    
    if (data == "") break;
    temp = "(";
    for (unsigned i = 0; i < data.length(); ++i){
      temp += data[i];
    }
  }
    break;

  case '^': //Update led bar graph
  {
    temp += "^";
    temp += data[0];
    temp += data[1];
  }
    break;

  case '&': //play sound effect
  {
    temp += "&";
    auto * msg = reinterpret_cast<PlaySoundMessage *>(data);
    temp += (char)strlen(msg->name);
    for (int i = 0; i < strlen(msg->name); ++i){
      temp += msg->name[i];
    }
  }
    break;
  case '*': //play 3D sound effect
  {
    temp += "*";
    auto * msg = reinterpret_cast<Play3DSoundMessage *>(data);
    std::string name = msg->name;
    float tempNum = msg->source->mPositionX();
    for (int k = 0; k < sizeof(float); ++k)
    {
      temp += static_cast<char *>(static_cast<void *>(&(tempNum)))[k];
    }
    tempNum = msg->source->mPositionY();
    for (int k = 0; k < sizeof(float); ++k)
    {
      temp += static_cast<char *>(static_cast<void *>(&(tempNum)))[k];
    }
    temp += (char)name.length();
    std::cout << name << " " << name.length() << std::endl;
    for (int i = 0; i < name.length(); ++i){
      temp += name[i];
    }
  }
    break;
  }
  return temp;
}
