#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <memory>
#include <queue>
#include <time.h>
#include "MCP3008SPI.h"
#include "GraphicsSystem.h"
#include "AudioSystem.h"
#include "NetworkingSystem.h"
#include "Object.h"
#include "GPIOPin.h"
#include <thread>
#include <fmod.h>
#include "Debug.h"

std::string inputstream = "";

DebugClass Debug("output.txt");

struct ThreadInfo{

char counter = 0;
GPIOPin * bit1;
GPIOPin * bit2;
int prevState = 0;
};
//mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);

std::unordered_map<unsigned int, Object*> gObjects[10];
std::unordered_map<unsigned int, Object*> gObjectMap;

unsigned int pID = -1;

int count[10];

GPIOPin *gpioPins[10];

bool ctrl_c_pressed = false;

void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}

bool Input ( void )
{
  static int first = 1;
  static int mouseFd = -1;
  static int keyboardFd = -1;
  static int timeout = 0;
  struct input_event ev[64];
  int rd;

  bool ret = true;
  //if(timeout++ > 20)return false;

  // Set up the devices on the first call
  if(first)
  {
    DIR *dirp;
    struct dirent *dp;
    regex_t kbd,mouse;

    char fullPath[1024];
    char *dirName = "/dev/input/by-id";
    int result;

    if(regcomp(&kbd,"event-kbd",0)!=0)
    {
        printf("regcomp for kbd failed\n");
        return false;

    }
    if(regcomp(&mouse,"event-mouse",0)!=0)
    {
        printf("regcomp for mouse failed\n");
        return false;

    }

    if ((dirp = opendir(dirName)) == NULL) {
        perror("couldn't open '/dev/input/by-id'");
        return false;
    }

    // Find any files that match the regex for keyboard or mouse

    do {
      errno = 0;
      if ((dp = readdir(dirp)) != NULL) 
      {
        printf("readdir (%s)\n",dp->d_name);
        if(regexec (&kbd, dp->d_name, 0, NULL, 0) == 0)
        {
            printf("match for the kbd = %s\n",dp->d_name);
            sprintf(fullPath,"%s/%s",dirName,dp->d_name);
            keyboardFd = open(fullPath,O_RDONLY | O_NONBLOCK);
            printf("%s Fd = %d\n",fullPath,keyboardFd);
            //printf("Getting exclusive access: ");
            //result = ioctl(keyboardFd, EVIOCGRAB, 1);
            //printf("%s\n", (result == 0) ? "SUCCESS" : "FAILURE");

        }
      }
    } while (dp != NULL);
    //std::cout<<"...."<<std::endl;
    closedir(dirp);


    regfree(&kbd);
    regfree(&mouse);

    
    first = 0;
    if((keyboardFd == -1)) return false;

  }

    // Read events from keyboard

    rd = read(keyboardFd,ev,sizeof(ev));
    //std::cout<<"read "<<rd<<std::endl;
    if(rd > 0)
    {
      int count,n;
      struct input_event *evp;

      count = rd / sizeof(struct input_event);
      n = 0;
      while(count--)
      {
        evp = &ev[n++];
        if(evp->type == 1)
        {
          if(evp->value == 0)
          {
            for(unsigned i = 0; i < sizeof(decltype(evp->code)); ++i)
              inputstream += ((unsigned char *)(&evp->code))[i];
            inputstream += '0';
          }
          else if(evp->value == 1)
          {
            for(unsigned i = 0; i < sizeof(decltype(evp->code)); ++i)
              inputstream += ((unsigned char *)(&evp->code))[i];
            inputstream += '1';
          }
          //std::cout<<evp->code<<" --- "<<evp->value<<std::endl;
          
          if((evp->code == KEY_Q) && (evp->value == 1))
              ret = false;
        }
      }

    }
    
    //std::cout<<a2d.GetChannelData(1)<<std::endl;
    //std::cout<<a2d.GetChannelData(0)<<std::endl;

    return(ret);
}


void GetClientNumber(int & pos, int & clientNumber, const char * buf)
{
  if(buf[pos] == '~')++pos;
  clientNumber = 0;
  while(buf[pos] != '~')
  {
    clientNumber *= 10;
    clientNumber += buf[pos++] - '0';
  }
}

std::queue<std::string> commands;
std::string unfinished = "";
unsigned short lastFrameSeen = 0;

#include <bitset>
void ProcessResponse(int& pos, int & clientNumber, const char * command, int len, GraphicsSystem * g, NetworkingSystem * n, AudioSystem * a)
{
  //for (int i = 0; i < len; ++i)
  //{
  //  if (buf[i] == '\0'){
  //    commands.push(unfinished);
  //    unfinished = "";
  //  }
  //  else
  //  {
  //    unfinished += buf[i];
  //  }
  //}
  pos = 0;
  while(pos < len){
    //std::string command = commands.front(); commands.pop();
    //Debug.Log("Command: " + command[pos]);
    switch(command[pos]){
      case 'L': //INITIAL Load
      {
        ++pos;
        Debug.Log("Loading an object");
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        Debug.Log("Object ID" + std::to_string(objectID));
        char isVis = command[pos];
        ++pos;

        char textureID = command[pos];
        ++pos;
        char r = command[pos];
        ++pos;
        char g = command[pos];
        ++pos;
        char b = command[pos];
        ++pos;
        char a = command[pos];
        ++pos;
        
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<" zPos: "<< zPos <<std::endl;
        pos += sizeof(float);
        const float xSca = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"["<<len <<" xSca: "<< xSca <<std::endl;
        pos += sizeof(float);
        const float ySca = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"*"<<len <<" ySca: "<< ySca <<std::endl;
        pos += sizeof(float);
        const float rot  = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"~"<<len <<" rot: "<< rot <<std::endl;
        pos += sizeof(float);
        if(gObjects[(int)zPos].find(objectID) == gObjects[(int)zPos].end())
        {
          Object * obj = new Object();
          gObjects[(int)zPos].insert({objectID, obj});
          gObjectMap.insert({objectID, obj});
        }
        Object * temp = gObjects[(int)zPos][objectID];
        temp->position[0] = xPos;
        temp->position[1] = yPos;
        temp->position[2] = zPos;
        temp->scale[0] = xSca;
        temp->scale[1] = ySca;
        temp->rotation = rot;
        temp->textureID = textureID;
        if(isVis == '0'){
          (temp)->inUse = false;
        }
        else{
          (temp)->inUse = true;
        }
        
        std::string tempstring = "L";
        for(unsigned i = 0; i < sizeof(unsigned int); ++i)
        {
          tempstring += static_cast<const unsigned char *>(static_cast<const void *>(&(objectID)))[i];
        } 
        n->Send(tempstring.data(), tempstring.length());
      }
      break;

      case '`': // Object created. 
      {
        ++pos;
        Debug.Log("Creating an object");
        std::cout<<"CREATE"<<std::endl;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        Debug.Log("Object ID" + std::to_string(objectID));
        pos += sizeof(unsigned int);
        std::cout<<"OID"<<std::endl;
        char isVis = command[pos];
        ++pos;
        std::cout<<"isVis"<<std::endl;
        char textureID = command[pos];
        ++pos;
        char r = command[pos];
        ++pos;
        char g = command[pos];
        ++pos;
        char b = command[pos];
        ++pos;
        char a = command[pos];
        ++pos;
        std::cout<<"TID"<<std::endl;
        
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<" zPos: "<< zPos <<std::endl;
        pos += sizeof(float);
        const float xSca = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"["<<len <<" xSca: "<< xSca <<std::endl;
        pos += sizeof(float);
        const float ySca = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"*"<<len <<" ySca: "<< ySca <<std::endl;
        pos += sizeof(float);
        const float rot  = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"~"<<len <<" rot: "<< rot <<std::endl;
        std::cout<<"PosScaRot"<<std::endl;
        pos += sizeof(float);
        auto temp = gObjectMap.find(objectID);
        if(temp == gObjectMap.end())
        {
          Object * obj = new Object();
          gObjects[(int)zPos].insert({objectID, obj});
          gObjectMap.insert({objectID, obj});
        }
        Debug.Log("Object Created");
        std::cout<<"OCreated"<<std::endl;
        temp = gObjectMap.find(objectID);

        temp->second->position[0] = xPos;
        temp->second->position[1] = yPos;
        temp->second->position[2] = zPos;
        temp->second->scale[0] = xSca;
        temp->second->scale[1] = ySca;
        temp->second->rotation = rot;
        temp->second->textureID = textureID;
        temp->second->r = r / 255.f;
        temp->second->g = g / 255.f;
        temp->second->b = b / 255.f;
        temp->second->a = a / 255.f;
        std::cout<<"OSetup"<<std::endl;
        if(isVis == '0'){
          (temp)->second->inUse = false;
        }
        else{
          (temp)->second->inUse = true;
        }
        
        std::string tempstring = "`";
        for(unsigned i = 0; i < sizeof(unsigned int); ++i)
        {
          tempstring += static_cast<const unsigned char *>(static_cast<const void *>(&(objectID)))[i];
        } 
        n->Send(tempstring.data(), tempstring.length());
        std::cout<<"CreateAcks"<<std::endl;
      }
      break;
      case '%': //Object died
      {
        ++pos;
        Debug.Log("Killing an object");
        std::cout<<"DED"<<std::endl;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        Debug.Log("Object ID" + std::to_string(objectID));
        pos += sizeof(unsigned int);
        
        if(gObjectMap.find(objectID) != gObjectMap.end()){
          gObjectMap[objectID]->inUse = false;
        }
        std::string temp = "%";
        for(unsigned i = 0; i < sizeof(unsigned int); ++i)
        {
          temp += static_cast<const unsigned char *>(static_cast<const void *>(&(objectID)))[i];
        } 
        n->Send(temp.data(), temp.length());
      }
      break;

      case '#': //Object moved
      {
        ++pos;
        Debug.Log("Moving an object");
        //std::cout<<"GOT MOVE MESSAGE"<<std::endl;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        char isVis = command[pos];
        ++pos;
        //std::cout<<isVis<<std::endl;
        
        auto temp = gObjectMap.find(objectID);
        
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<" zPos: "<< zPos <<std::endl;
        pos += sizeof(float);
        const float xSca = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"["<<len <<" xSca: "<< xSca <<std::endl;
        pos += sizeof(float);
        const float ySca = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"*"<<len <<" ySca: "<< ySca <<std::endl;
        pos += sizeof(float);
        const float rot  = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"~"<<len <<" rot: "<< rot <<std::endl;
        pos += sizeof(float);
        if(temp != gObjectMap.end()){
          if(isVis == '0'){
            (temp)->second->inUse = false;
            //std::cout<<"Something is invisible"<<std::endl;
          }
          else{
            (temp)->second->inUse = true;
          }
          (temp)->second->position[0] = xPos;
          (temp)->second->position[1] = yPos;
          (temp)->second->position[2] = zPos;
          (temp)->second->scale[0] = xSca;
          (temp)->second->scale[1] = ySca;
          (temp)->second->rotation = rot;
        }
        
      }
      break;
      case '(': //Move camera
      {
        Debug.Log("Moving Camera");
        ++pos;
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        g->mMainCamera.x = xPos;
        g->mMainCamera.y = yPos;
        g->viewChanged = true;
      }
      break;
      case '$': //Object texture changed
      {
        ++pos;
        Debug.Log("Changing sprite texture");
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        char textureID = command[pos];
        ++pos;
        char r = command[pos];
        ++pos;
        char g = command[pos];
        ++pos;
        char b = command[pos];
        ++pos;
        char a = command[pos];
        ++pos;
        auto temp = gObjectMap.find(objectID);
        if(temp != gObjectMap.end()){
          temp->second->textureID = textureID;
          temp->second->r = r / 255.f;
          temp->second->g = g / 255.f;
          temp->second->b = b / 255.f;
          temp->second->a = a / 255.f;
        }
      }
      break;

      case '^': //Update led bar graph
      {
        ++pos;
        Debug.Log("Updating LEDS");
        char d[2];
        d[0] = *reinterpret_cast<const char*>(&(command[pos]));
        ++pos;
        d[1] = *reinterpret_cast<const char*>(&(command[pos]));
        ++pos;
        
        std::bitset<8> d1(d[0]);
        std::bitset<8> d2(d[1]);

        for(int i = 0; i < 8; ++i){
          gpioPins[i]->SetPinVal((d1[i] != 0) ? "1" : "0");
        }
        for(int i = 0; i < 2; ++i){
          gpioPins[8 + i]->SetPinVal((d2[i] != 0) ? "1" : "0");
        }
      }
      break;

      case '&': //play sound effect
      {
        ++pos;
        Debug.Log("Playing global sound effect");
        const char length = command[pos++];
        std::string name;
        for(int i = 0; i < length; ++i){
          name += command[pos++];
        }
        a->PlaySoundEffect(name);
      }
      break;
      case '*':
      {
        ++pos;
        Debug.Log("Playing 3D sound effect");
        const float sourcePosX = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        std::cout<<sourcePosX<<std::endl;
        const float sourcePosY = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        std::cout<<sourcePosY<<std::endl;
        pos += sizeof(float);
        std::cout<<(int)command[pos]<<std::endl;
        const char length = command[pos++];
        std::string name;
        for(int i = 0; i < length; ++i){
          std::cout<<command[pos]<<std::endl;
          name += command[pos++];
        }
        std::cout<<(int)length<<" "<<name<<std::endl;
        a->Play3DSoundEffect(name, sourcePosX, sourcePosY, g->mMainCamera.x, g->mMainCamera.y);
      }
      break;
    }
  }
}

void KnobTurned(ThreadInfo * t)
{
  while(!ctrl_c_pressed){
    //std::cout<<"CALLED"<<std::endl;
    std::string b1, b2;
    b1 = t->bit1->GetPinVal();
    b2 = t->bit2->GetPinVal();
    //std::cout<<b1<<"  "<<b2<<std::endl;
    int num = (b1 == "1") ? (1<<1) : (0<<1);
    num |= (b2 == "1") ? (1) : (0);
    if(num == t->prevState){
      //std::cout<<"No mov";
      continue;
    }
    switch(num){
      case 0b00:
      if(t->prevState == 0b01){// && prevprev == 0b11){
        --t->counter;
      }
      else if(t->prevState == 0b10){// && prevprev == 0b11){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
      case 0b01:
      if(t->prevState == 0b11){// && prevprev == 0b10){
        --t->counter;
      }
      else if(t->prevState == 0b00){// && prevprev == 0b10){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
      case 0b11:
      if(t->prevState == 0b10){// && prevprev == 0b00){
        --t->counter;
      }
      else if(t->prevState == 0b01){// && prevprev == 0b00){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
      case 0b10:
      if(t->prevState == 0b00){// && prevprev == 0b01){
        --t->counter;
      }
      else if(t->prevState == 0b11){// && prevprev == 0b01){
        ++t->counter;
      }
      else{
        if(t->counter > 0)
          ++t->counter;
        if(t->counter < 0)
          --t->counter;
      }
      break;
    }
    t->prevState = num;
  }
  return;
}

int main ( int argc, char *argv[] )
{
  if(argc < 2){
    std::cout<<"Please identify which controller this is.( cone gun radar turret )"<<std::endl;
    return 0;
  }
  if(argc == 2){
    std::cout<<"TUrning off debug"<<std::endl;
    Debug.TurnOff();
  }
  else{
    std::cout<<"Starting debugging"<<std::endl;
    Debug.Clear();
  }
  int incrementer = 1;
  clock_t start, end;
  struct sigaction sig_struct;
  sig_struct.sa_handler = sig_handler;
  sig_struct.sa_flags = 0;
  sigemptyset(&sig_struct.sa_mask);

  if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
      std::cout << "Problem with sigaction" << std::endl;
      exit(1);
  }

  ThreadInfo threadInfo;
  char myID = -1;
  
  if(strcmp(argv[1], "cone")==0){
    myID = 0;
  }
  else if(strcmp(argv[1], "gun")==0){
    myID = 01;
  }
  else if(strcmp(argv[1], "radar")==0){
    myID = 02;
  }
  else if(strcmp(argv[1], "turret")==0){
    myID = 03;
  }
  else{
    std::cout<<"Controller name not found.( cone gun radar turret )"<<std::endl;
    return 0;
  } 

  mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);
  
  std::string PINS[] = {"27","17","18","23","24","25","12","16","20","21"};
  threadInfo.bit1 = new GPIOPin("5");
  threadInfo.bit2 = new GPIOPin("6");
  GPIOPin p("13");
  p.ExportPin();
  p.SetPinDir("out");
  threadInfo.bit1->ExportPin();
  threadInfo.bit1->SetPinDir("in");
  threadInfo.bit2->ExportPin();
  threadInfo.bit2->SetPinDir("in");
  
  for(int i = 0; i < 10; ++i){
    gpioPins[i] = new GPIOPin(PINS[i]);
    gpioPins[i]->ExportPin();
    gpioPins[i]->SetPinDir("out");
    gpioPins[i]->SetPinVal("1");
  }


  AudioSystem a;
  //a.Shutdown();
  //return 0;
  GraphicsSystem g;
  NetworkingSystem n(27015, "192.168.77.106");
  std::cout<<"CONNECTED"<<std::endl;
  std::string hellomsg("+");
  hellomsg += myID;
  int res = n.Send(hellomsg.c_str(), 2);
  std::cout<<hellomsg[0]<<(int)hellomsg[1]<<std::endl;
  //return 0;
  g.LoadTextures("../Assets/Textures.JSON");

  //for(auto & iter : g.mTextures){
  // // std::cout<<iter.first<<"   "<<iter.second.name<<std::endl;
  //}
  
  bool toSend = false;
  char buf[1024] = {0};
  int pos = 0;
  int clientNumber = -1;
  int netResult = 0;
  int state = 0;
  int prevState = 0;
  float deltatime, gDt, rDt,sDt,iDt;
  std::thread t1(KnobTurned, &threadInfo);
  while(true){
    start = clock();
    a.Update(0.016);
    //for(int i = 0; i < 10; ++i){
    //  if(i == counter)
    //    gpioPins[i]->SetPinVal("1");
    //  else{
    //    gpioPins[i]->SetPinVal("0");
    //  }
    //}
    //if(counter >= 10) counter = 0;
    //if(counter <= -1) counter = 9;
    //std::cout<<"loop"<<std::endl;
    bool updated = false;
    //gettimeofday ( &tStart , &tz );
    do{
      memset((void*)buf, 0, 1024);
      //std::cout<<"Tryna recv"<<std::endl;
      netResult = n.Receive((buf),1023);
      
      
      pos = 0;
      if(netResult > 0)
      {
        //std::cout<<"netResult: "<<netResult<<std::endl;
        ProcessResponse(pos, clientNumber, buf, netResult, &g, &n, &a);
      }
    }while(netResult > 0);
    //gettimeofday ( &tEnd , &tz );
    //rDt = (float)(tEnd.tv_sec - tStart.tv_sec + (tEnd.tv_usec - tStart.tv_usec) * 1e-6);
    //gettimeofday ( &tStart , &tz );
    //g.Draw();
    std::cout<<"PIN 13 VAL:         "<<p.GetPinVal()<<std::endl;
    //gettimeofday ( &tEnd , &tz );
    //gDt = (float)(tEnd.tv_sec - tStart.tv_sec + (tEnd.tv_usec - tStart.tv_usec) * 1e-6);
    toSend = !toSend;
    inputstream = "~";
    //gettimeofday ( &tStart , &tz );
    unsigned short x1 = a2d.GetChannelData(0);
    unsigned short y1 = a2d.GetChannelData(1);
    unsigned short x2 = a2d.GetChannelData(2);
    unsigned short y2 = a2d.GetChannelData(3);
    for(unsigned i = 0; i < sizeof(unsigned short); ++i){
      inputstream += static_cast<char *>(static_cast<void *>(&x1))[i];
    }
    for(unsigned i = 0; i < sizeof(unsigned short); ++i){
      inputstream += static_cast<char *>(static_cast<void *>(&y1))[i];
    }
    for(unsigned i = 0; i < sizeof(unsigned short); ++i){
      inputstream += static_cast<char *>(static_cast<void *>(&x2))[i];
    }
    for(unsigned i = 0; i < sizeof(unsigned short); ++i){
      inputstream += static_cast<char *>(static_cast<void *>(&y2))[i];
    }
    inputstream += (a2d.GetChannelData(4) > 15) ? '0' : '1';
    inputstream += (a2d.GetChannelData(5) > 15) ? '0' : '1';
    
    std::cout<<"("<<x1<<","<<y1<<")("<<x2<<","<<y2<<")"<<std::endl;


    inputstream += threadInfo.counter;
    threadInfo.counter = 0;
    if(toSend && inputstream.length() > 0){
      
      //inputstream = "~" + inputstream + "!";
      std::vector<char> v(inputstream.length() + 1);
      for(unsigned i = 0; i < inputstream.length(); ++i)v[i] = inputstream[i];
      char* pc = &v[0];
      //std::cout<<inputstream<<std::endl;
      int sentbytes = n.Send(pc, inputstream.length());
      //std::cout<<"Bytes sent: "<<sentbytes<<inputstream<<std::endl;
      inputstream = "";
    }
    
    deltatime = clock() - start;
    deltatime /= CLOCKS_PER_SEC;
    //std::cout<<deltatime<<std::endl;
    if(deltatime >= 1.0f/30.f)
    {
      //std::cout<<"Frame took too long ";
      
    }
    if(ctrl_c_pressed)
    {
        std::cout << "Ctrl^C Pressed" << std::endl;
        std::cout << "unexporting pins" <<std::endl;
        t1.join();
        //gpio4->unexport_gpio();
        //gpio17->unexport_gpio();
        //cout << "deallocating GPIO Objects" << endl;
        //delete gpio4;
        //gpio4 = 0;
        //delete gpio17;
        //gpio17 =0;
        for(int i = 0; i < 10; ++i){
          gpioPins[i]->SetPinVal("0");
          gpioPins[i]->UnexportPin();
          delete gpioPins[i];
          gpioPins[i] = 0;
        }
        p.UnexportPin();
        threadInfo.bit1->UnexportPin();
        delete threadInfo.bit1;
        threadInfo.bit1 = 0;
        threadInfo.bit2->UnexportPin();
        delete threadInfo.bit2;
        threadInfo.bit2 = 0;
        break;

    }

  }
  
  return 0;
}
