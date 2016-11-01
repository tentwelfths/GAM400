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
#include "MCP3008SPI.h"
#include "GraphicsSystem.h"
#include "NetworkingSystem.h"
#include "Object.h"
#include "GPIOPin.h"

std::string inputstream = "";
//mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);

std::unordered_map<unsigned int, Object*> gObjects[50];
std::unordered_map<unsigned int, Object*> gObjectMap;

unsigned int pID = -1;

int count[50];

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
void ProcessResponse(int& pos, int & clientNumber, const char * command, int len, GraphicsSystem * g, NetworkingSystem * n)
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
    switch(command[pos]){
      case 'L': //INITIAL Load
      {
        ++pos;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        const unsigned char textureID = *reinterpret_cast<const unsigned char*>(&(command[pos]));
        pos += sizeof(unsigned char);
        if(textureID == 3){
          std::cout<<"Found player " << objectID<<std::endl;
          pID = objectID;
        }
        else if(objectID == pID){
          std::cout<<"GOT LOAD MESSAGE" << objectID <<std::endl;
          std::cout<<"GOT A FUCKIN DUPE! "<<textureID<<std::endl;
        }
        //std::string textureName = "";
        //for(unsigned char i = 0; i < textureNameLength; ++i){
        //  textureName += (char)command[pos++];
        //}
        
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"]"<<len <<" zPos: "<< zPos <<std::endl;
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
        if(gObjects[textureID].find(objectID) == gObjects[textureID].end())
        {
          Object * obj = new Object();
          gObjects[textureID].insert({objectID, obj});
          gObjectMap.insert({objectID, obj});
        }
        Object * temp = gObjects[textureID][objectID];
        temp->position[0] = xPos;
        temp->position[1] = yPos;
        temp->position[2] = zPos;
        temp->scale[0] = xSca;
        temp->scale[1] = ySca;
        temp->rotation[2] = rot;
        temp->textureID = textureID;
        temp->inUse = true;
        
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
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        
        const unsigned char textureID = *reinterpret_cast<const unsigned char*>(&(command[pos]));
        pos += sizeof(unsigned char);
        if(textureID == 3){
          std::cout<<"Found player " << objectID<<std::endl;
          pID = objectID;
        }
        else if(objectID == pID){
          std::cout<<"GOT CREATE MESSAGE" << objectID <<std::endl;
          std::cout<<"GOT A FUCKIN DUPE! "<<textureID<<std::endl;
        }
        //std::string textureName = "";
        //for(unsigned char i = 0; i < textureNameLength; ++i){
        //  textureName += (char)command[pos++];
        //}
        
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"]"<<len <<" zPos: "<< zPos <<std::endl;
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
        if(gObjects[textureID].find(objectID) == gObjects[textureID].end())
        {
          Object * obj = new Object();
          gObjects[textureID].insert({objectID, obj});
          gObjectMap.insert({objectID, obj});
        }
        Object * temp = gObjects[textureID][objectID];
        temp->position[0] = xPos;
        temp->position[1] = yPos;
        temp->position[2] = zPos;
        temp->scale[0] = xSca;
        temp->scale[1] = ySca;
        temp->rotation[2] = rot;
        temp->textureID = textureID;
        temp->inUse = true;
        
        std::string tempstring = "`";
        for(unsigned i = 0; i < sizeof(unsigned int); ++i)
        {
          tempstring += static_cast<const unsigned char *>(static_cast<const void *>(&(objectID)))[i];
        } 
        n->Send(tempstring.data(), tempstring.length());
      }
      break;
      case '%': //Object died
      {
        ++pos;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        
        std::cout<<objectID<<std::endl;
        if(pID == objectID){
          std::cout<<"GOT DED MESSAGE -- PLAYER DED???????"<<std::endl;
        }
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
        //std::cout<<"GOT MOVE MESSAGE"<<std::endl;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        char isVis = command[pos];
        ++pos;
        //std::cout<<isVis<<std::endl;
        if(isVis == '0'){
          //gObjectMap[objectID]->inUse = false;
        }
        else{
          //gObjectMap[objectID]->inUse = true;
        }
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"]"<<len <<" zPos: "<< zPos <<std::endl;
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
        auto temp = gObjectMap.find(objectID);
        if(temp != gObjectMap.end()){
          (temp)->second->position[0] = xPos;
          (temp)->second->position[1] = yPos;
          (temp)->second->position[2] = zPos;
          (temp)->second->scale[0] = xSca;
          (temp)->second->scale[1] = ySca;
          (temp)->second->rotation[2] = rot;
        }
        
      }
      break;
      case '(': //Move camera
      {
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        //std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        g->mMainCamera.x = xPos;
        g->mMainCamera.y = yPos;
      }
      break;
      case '$': //Object texture changed
      {
        ++pos;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        pos += sizeof(unsigned int);
        char textureID = command[pos];
        ++pos;
        Object * obj = gObjects[gObjectMap[objectID]->textureID][objectID];
        gObjects[gObjectMap[objectID]->textureID].erase(gObjects[gObjectMap[objectID]->textureID].find(objectID));
        obj->textureID = textureID;
        gObjects[textureID].insert({objectID, obj});
      }
      break;

      case '^': //Update led bar graph
      {
        ++pos;
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
      }
      break;

    }
  }
}

int KnobTurned(GPIOPin * bit1, GPIOPin * bit2, int& prevState)
{
  std::string b1, b2;
  int counter = 0;
  b1 = bit1->GetPinVal();
  b2 = bit2->GetPinVal();
  //std::cout<<b1<<"  "<<b2<<std::endl;
  int num = (b1 == "1") ? (1<<1) : (0<<1);
  num |= (b2 == "1") ? (1) : (0);
  if(num == prevState){
    return 0;
  }
  switch(num){
    case 0b00:
    if(prevState == 0b01){
      --counter;
    }
    else{
      ++counter;
    }
    break;
    case 0b01:
    if(prevState == 0b11){
      --counter;
    }
    else{
      ++counter;
    }
    break;
    case 0b11:
    if(prevState == 0b10){
      --counter;
    }
    else{
      ++counter;
    }
    break;
    case 0b10:
    if(prevState == 0b00){
      --counter;
    }
    else{
      ++counter;
    }
    break;
  }
  prevState = num;
  return counter;
}

int main ( int argc, char *argv[] )
{
  if(argc < 2){
    std::cout<<"Please identify which controller this is.( cone gun radar turret )"<<std::endl;
    return 0;
  }
  int incrementer = 1;
  
  struct sigaction sig_struct;
  sig_struct.sa_handler = sig_handler;
  sig_struct.sa_flags = 0;
  sigemptyset(&sig_struct.sa_mask);

  if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
      std::cout << "Problem with sigaction" << std::endl;
      exit(1);
  }

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
  GPIOPin * bit1 = new GPIOPin("5");
  GPIOPin * bit2 = new GPIOPin("6");
  bit1->ExportPin();
  bit1->SetPinDir("in");
  bit2->ExportPin();
  bit2->SetPinDir("in");
  char counter = 5;
  for(int i = 0; i < 10; ++i){
    gpioPins[i] = new GPIOPin(PINS[i]);
    gpioPins[i]->ExportPin();
    gpioPins[i]->SetPinDir("out");
    if(i == counter)
      gpioPins[i]->SetPinVal("1");
    else{
      gpioPins[i]->SetPinVal("0");
    }
  }


  GraphicsSystem g;
  NetworkingSystem n(27015, "192.168.77.106");
  std::cout<<"CONNECTED"<<std::endl;
  int res = n.Send(std::string("+" + myID).c_str(), 2);
  //std::cout<<res<<std::endl;
  //return 0;
  g.LoadTextures("../Assets/Textures.JSON");

  for(auto & iter : g.mTextures){
    std::cout<<iter.first<<"   "<<iter.second.name<<std::endl;
  }
  
  bool toSend = false;
  char buf[1024] = {0};
  int pos = 0;
  int clientNumber = -1;
  int netResult = 0;
  struct timeval t1, t2;
  struct timeval tStart,tEnd;
  struct timezone tz;
  int state = 0;
  float deltatime, gDt, rDt,sDt,iDt;
  while(true){

    //for(int i = 0; i < 10; ++i){
    //  if(i == counter)
    //    gpioPins[i]->SetPinVal("1");
    //  else{
    //    gpioPins[i]->SetPinVal("0");
    //  }
    //}
    //if(counter >= 10) counter = 0;
    //if(counter <= -1) counter = 9;
    counter = KnobTurned(bit1, bit2, state);
    //std::cout<<"loop"<<std::endl;
    gettimeofday ( &t1 , &tz );
    bool updated = false;
    gettimeofday ( &tStart , &tz );
    do{
      memset((void*)buf, 0, 1024);
      //std::cout<<"Tryna recv"<<std::endl;
      netResult = n.Receive((buf),1023);
      
      
      pos = 0;
      if(netResult > 0)
      {
        //std::cout<<"netResult: "<<netResult<<std::endl;
        ProcessResponse(pos, clientNumber, buf, netResult, &g, &n);
      }
    }while(netResult > 0);
    gettimeofday ( &tEnd , &tz );
    rDt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
    gettimeofday ( &tStart , &tz );
    g.Draw();
    gettimeofday ( &tEnd , &tz );
    gDt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
    toSend = !toSend;
    inputstream = "~";
    gettimeofday ( &tStart , &tz );
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
    inputstream += (a2d.GetChannelData(5) > 15) ? '0' : '1';
    inputstream += counter;
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
    gettimeofday ( &tEnd , &tz );
    iDt = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
    
    gettimeofday(&t2, &tz);
    deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec)/1000.f);
    if(deltatime >= 1.0f/30.f)
    {
      //Frame took too long
      float total = gDt + rDt + iDt;
      std::cout<<"Frame took too long ";
      std::cout<<deltatime<<std::endl;
      std::cout<<"Graphics: " << (gDt / total) * 100.f <<"%"<<std::endl
      <<"Input: " << (iDt / total) * 100.f <<"%"<<std::endl
      <<"Receiving: " << (rDt / total) * 100.f <<"%"<<std::endl<<std::endl;
      
    }
    //do{
    //  gettimeofday(&t2, &tz);
    //  deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
    //}while(deltatime < 1.0f/30.0f);
    if(ctrl_c_pressed)
    {
        std::cout << "Ctrl^C Pressed" << std::endl;
        std::cout << "unexporting pins" <<std::endl;
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
        bit1->UnexportPin();
        delete bit1;
        bit1 = 0;
        bit2->UnexportPin();
        delete bit2;
        bit2 = 0;
        break;

    }

  }
  
  return 0;
}
