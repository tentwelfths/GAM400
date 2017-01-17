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
#include "Controllers.h"

std::string inputstream = "";

IController * controller;

DebugClass Debug("output.txt");

bool LevelLoaded = false;
bool LevelLoadingInProcess = false;
std::string LevelFilename = "";


// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v,")
{
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v,")
{
  s.erase(0, s.find_first_not_of(t));
  return s;
}

// trim from both ends of string (left & right)
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v,")
{
  return ltrim(rtrim(s, t), t);
}

//mcp3008Spi a2d("/dev/spidev0.0", SPI_MODE_0, 1000000, 8);

std::unordered_map<unsigned int, Object*> gObjects[10];
std::unordered_map<unsigned int, Object*> gObjectMap;

std::vector<Object*> unusedObjects;

unsigned int pID = -1;

int count[10];

GPIOPin *gpioPins[10];

bool ctrl_c_pressed = false;

void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}

void RemoveObject(unsigned objectID){
  Object * obj = gObjectMap[objectID];
  gObjects[(int)obj->position[2]].erase(gObjects[(int)obj->position[2]].find(objectID));
  gObjectMap.erase(gObjectMap.find(objectID));
  unusedObjects.push_back(obj);
}

void UnloadLevel(){
  for(auto& iter : gObjectMap){
    Object * obj = iter.second;
    gObjects[(int)obj->position[2]].erase(gObjects[(int)obj->position[2]].find(obj->ID));
    gObjectMap.erase(gObjectMap.find(obj->ID));
    unusedObjects.push_back(obj);
  }
}

void GetTransformFromFile(Object* obj, std::ifstream & file){
  std::string line;
  std::getline(file,line);//mPosition
  std::getline(file,line);//x: 1.000000
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->position[0] = std::stof(line);
  std::getline(file,line);//y: 2.000000
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->position[1] = std::stof(line);
  std::getline(file,line);//z: 1.000000
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->position[2] = std::stof(line);
	std::getline(file,line);//		},
	std::getline(file,line);//	"mScale_": {
	std::getline(file,line);//		x: 1.000000,
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->scale[0] = std::stof(line);
	std::getline(file,line);//		y: 2.000000,
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->scale[1] = std::stof(line);
	std::getline(file,line);//		z: 1.000000
	std::getline(file,line);//		},
	std::getline(file,line);//	"mRotation_": {
	std::getline(file,line);//		x: 0.000000,
	std::getline(file,line);//		y: 0.000000,
	std::getline(file,line);//		z: 45.000000
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  obj->rotation = std::stof(line);
  obj->inUse = true;
	std::getline(file,line);//		}
	std::getline(file,line);//	},
}

void GetSpriteFromFile(Object* obj, std::ifstream & file,GraphicsSystem * g){
  std::string line;
  std::getline(file,line);//	"mTextureName": car.png,
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  std::cout<<line<<std::endl;
  for(auto &iter : g->mTextures){
    if(iter.second.name == line){
      obj->textureID = iter.first;
      break;
    }
  }
	std::getline(file,line);//	"mOpacity_": 1.000000,
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  char a = line[0];
  std::cout<<(int)a<<std::endl;
	std::getline(file,line);//	"mTint_": {
	std::getline(file,line);//			x: 0.000000,
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  char r =  line[0];
  std::cout<<(int)r<<std::endl;
	std::getline(file,line);//			y: 0.000000,
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  char gr =  line[0];
  std::cout<<(int)gr<<std::endl;
	std::getline(file,line);//			z: 0.000000
  line = trim(line);
  line = line.substr(line.find_first_of(':') + 2);
  char b =  line[0];
  std::cout<<(int)b<<std::endl;
	std::getline(file,line);//			}
	std::getline(file,line);//	},


  obj->r = r / 255.f;
  obj->g = gr / 255.f;
  obj->b = b / 255.f;
  obj->a = a / 255.f;
}

void DeserializeComponent(Object* obj, std::string componentName, std::ifstream & file,GraphicsSystem * g)
{
  std::string line;
  if(componentName == "TransformComponent"){
    std::cout<<"GETTING TRANSFORM"<<std::endl;
    GetTransformFromFile(obj, file);
    return;
  }
  else if(componentName == "SpriteComponent"){
    std::cout<<"GETTING SPRITE"<<std::endl;
    GetSpriteFromFile(obj, file,g);
    return;
  }
  std::cout<<"GETTING OTHER COMPONENT"<<std::endl;
  int bracketCount = 1;
  while (bracketCount > 0)
  {
    std::getline(file,line);
    line = trim(line);
    //std::cout<<line<<std::endl;
    if (line == "}" || line=="},"){
      bracketCount -= 1;
      continue;
    }
    std::string memberName = "";
    std::string value = "";
    unsigned i = 0;
    char state = 0;
    bool complex = false;
    for (; i < line.length(); ++i){
      if (line[i] == '\"'){
        if (state == 0){
          state = 1;
        }
        else if (state == 1){
          state = 3;
        }
        continue;
      }
      else if (state == 2 && line[i] == '\"'){
        state = 3;
      }
      else if (state == 3){
        if (line[i] == ','){
          break;
        }
        else if (line[i] == '{'){
          bracketCount += 1;
        }
        else{
          if(line[i] == ':')continue;
          value += line[i];
        }
      }
      if (state == 1){
        memberName += line[i];
      }
    }
  }
  std::cout<<"DONE GETTING OTHER COMPONENT"<<std::endl;
}

Object * DeserializeObject(std::ifstream & file,GraphicsSystem * g)
{
  Object * obj;// = new Object();
  if(unusedObjects.size() > 0){
    obj = unusedObjects.back();
    unusedObjects.pop_back();
  }
  else{
    obj = new Object();
  }

  std::string line;
  std::getline(file, line);
  line = trim(line);
  if (line.find_first_of(':') != std::string::npos && line.substr(0, line.find_first_of(':')) == "ObjectID")
  {
    line = line.substr(line.find_first_of(':') + 2);
    obj->ID = std::stoi(line);
    std::getline(file, line);
  }
  else{
    obj->ID = 0;
  }
  std::getline(file, line);
  std::getline(file, line);
  line = trim(line);
  while (!file.eof())
  {
    std::getline(file, line);
    line = trim(line);
    if (line == "{")continue;
    if (line == "}" || line == "},")
      break;
    line = line.substr(0, line.find_first_of(':'));
    DeserializeComponent(obj, line, file, g);
  }
  return obj;

}

void LoadLevel(GraphicsSystem * g){
  std::ifstream file("../Assets/" + LevelFilename);
  if(!file.is_open()){
    std::cout<<"FILE NOT OPEN"<<std::endl;
    return;
  }
  while (!file.eof())
  {
    std::string line;
    line += file.peek();
    if (line == "{"){
      std::getline(file, line);
      continue;
    }
    if (line == "}" || line == "},")
      break;
    std::cout<<"DESERIALIZE OBJECT"<<std::endl;
    Object * obj = DeserializeObject(file, g);
    gObjects[(int)obj->position[2]].insert({obj->ID, obj});
    gObjectMap.insert({obj->ID, obj});
  }

  LevelLoaded = true;
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
    ////std::cout<<"...."<<std::endl;
    closedir(dirp);


    regfree(&kbd);
    regfree(&mouse);

    
    first = 0;
    if((keyboardFd == -1)) return false;

  }

    // Read events from keyboard

    rd = read(keyboardFd,ev,sizeof(ev));
    ////std::cout<<"read "<<rd<<std::endl;
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
          ////std::cout<<evp->code<<" --- "<<evp->value<<std::endl;
          
          if((evp->code == KEY_Q) && (evp->value == 1))
              ret = false;
        }
      }

    }
    
    ////std::cout<<a2d.GetChannelData(1)<<std::endl;
    ////std::cout<<a2d.GetChannelData(0)<<std::endl;

    return(ret);
}

std::queue<std::string> commands;
std::string unfinished = "";
unsigned short lastFrameSeen = 0;

#include <bitset>
void ProcessResponse(int& pos,  const char * command, int len, GraphicsSystem * g, NetworkingSystem * n, AudioSystem * a)
{
  pos = 0;
  //std::cout<<"\t\t"<<len<<std::endl;
  while(pos < len){
    //std::string command = commands.front(); commands.pop();
    //Debug.Log("Command: " + command[pos]);
    switch(command[pos]){
      case 'L': //INITIAL Load
      {
        ++pos;
        std::cout<<"GOT THE L"<<std::endl;
        char length = command[pos++];
        LevelFilename = "";
        LevelLoadingInProcess = true;
        LevelLoaded = false;
        for(unsigned i = 0; i < length; ++i){
          LevelFilename += command[pos++];
        }
        std::cout<<LevelFilename<<std::endl;
        std::string tempstring = "L";
        tempstring += length;
        for(unsigned i = 0; i < LevelFilename.length(); ++i)
        {
          tempstring += LevelFilename[i];
        } 
        n->Send(tempstring.data(), tempstring.length());
      }
      break;
      case 'M': //But did you load though?
      {
        ++pos;
        std::cout<<"GOT THE M "<<LevelLoaded <<" " << LevelLoadingInProcess<<std::endl;
        if(!LevelLoaded && LevelLoadingInProcess){
          std::cout<<"LOADING"<<LevelFilename<<std::endl;
          UnloadLevel();
          std::cout<<"UNLOADED"<<std::endl;
          LoadLevel(g);
          std::cout<<"LOADED "<<LevelFilename<<std::endl;
          LevelLoaded = true;
        }
        std::string tempstring = "M";
        n->Send(tempstring.data(), tempstring.length());
        break;
      }
      case '`': // Object created. 
      {
        ++pos;
        std::cout<<"CREATE"<<std::endl;
        Debug.Log("Creating an object");
        //std::cout<<"CREATE"<<std::endl;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        Debug.Log("Object ID" + std::to_string(objectID));
        pos += sizeof(unsigned int);
        //std::cout<<"OID"<<std::endl;
        char isVis = command[pos];
        ++pos;
        //std::cout<<"isVis"<<std::endl;
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
        //std::cout<<"TID"<<std::endl;
        
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<" zPos: "<< zPos <<std::endl;
        pos += sizeof(float);
        const float xSca = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"["<<len <<" xSca: "<< xSca <<std::endl;
        pos += sizeof(float);
        const float ySca = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"*"<<len <<" ySca: "<< ySca <<std::endl;
        pos += sizeof(float);
        const float rot  = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"~"<<len <<" rot: "<< rot <<std::endl;
        //std::cout<<"PosScaRot"<<std::endl;
        pos += sizeof(float);
        auto temp = gObjectMap.find(objectID);
        if(temp == gObjectMap.end())
        {
          Object * obj;// = new Object();
          if(unusedObjects.size() > 0){
            obj = unusedObjects.back();
            unusedObjects.pop_back();
          }
          else{
            obj = new Object();
          }
          gObjects[(int)zPos].insert({objectID, obj});
          gObjectMap.insert({objectID, obj});
        }
        Debug.Log("Object Created");
        //std::cout<<"OCreated"<<std::endl;
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
        //std::cout<<"OSetup"<<std::endl;
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
        //std::cout<<"CreateAcks"<<std::endl;
      }
      break;
      case '%': //Object died
      {
        ++pos;
        Debug.Log("Killing an object");
        //std::cout<<"DED"<<std::endl;
        unsigned int objectID = *static_cast<const unsigned int *>(static_cast<const void *>(&(command[pos])));
        Debug.Log("Object ID" + std::to_string(objectID));
        pos += sizeof(unsigned int);
        
        if(gObjectMap.find(objectID) != gObjectMap.end()){
          gObjectMap[objectID]->inUse = false;
          RemoveObject(objectID);
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
        ////std::cout<<isVis<<std::endl;
        
        auto temp = gObjectMap.find(objectID);
        
        const float xPos = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        pos += sizeof(float);
        const float zPos = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<" zPos: "<< zPos <<std::endl;
        pos += sizeof(float);
        const float xSca = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"["<<len <<" xSca: "<< xSca <<std::endl;
        pos += sizeof(float);
        const float ySca = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"*"<<len <<" ySca: "<< ySca <<std::endl;
        pos += sizeof(float);
        const float rot  = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"~"<<len <<" rot: "<< rot <<std::endl;
        pos += sizeof(float);
        if(temp != gObjectMap.end()){
          if(isVis == '0'){
            (temp)->second->inUse = false;
            ////std::cout<<"Something is invisible"<<std::endl;
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
        ////std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        const float yPos = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
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
          controller->mLeds[i].SetPinVal((d1[i] != 0) ? "1" : "0");
        }
        for(int i = 0; i < 2; ++i){
          controller->mLeds[8 + i].SetPinVal((d2[i] != 0) ? "1" : "0");
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
        ////std::cout<<pos<<"+"<<len <<" xPos: "<< xPos <<std::endl;
        pos += sizeof(float);
        //std::cout<<sourcePosX<<std::endl;
        const float sourcePosY = *reinterpret_cast<const float*>(&(command[pos]));
        ////std::cout<<pos<<"="<<len <<" yPos: "<< yPos <<std::endl;
        //std::cout<<sourcePosY<<std::endl;
        pos += sizeof(float);
        //std::cout<<(int)command[pos]<<std::endl;
        const char length = command[pos++];
        std::string name;
        for(int i = 0; i < length; ++i){
          //std::cout<<command[pos]<<std::endl;
          name += command[pos++];
        }
        //std::cout<<(int)length<<" "<<name<<std::endl;
        a->Play3DSoundEffect(name, sourcePosX, sourcePosY, g->mMainCamera.x, g->mMainCamera.y);
      }
      break;
      default:
      Debug.Log("What the fuck is " + std::to_string(command[pos]));
      break;
    }
  }
}

int main ( int argc, char *argv[] )
{
  if(argc < 2){
    //std::cout<<"Please identify which controller this is.( cone gun radar turret )"<<std::endl;
    return 0;
  }
  if(argc == 2){
    //std::cout<<"TUrning off debug"<<std::endl;
    Debug.TurnOff();
  }
  else{
    //std::cout<<"Starting debugging"<<std::endl;
    Debug.Clear();
  }
  
  clock_t start, end;
  struct sigaction sig_struct;
  sig_struct.sa_handler = sig_handler;
  sig_struct.sa_flags = 0;
  sigemptyset(&sig_struct.sa_mask);

  if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
      //std::cout << "Problem with sigaction" << std::endl;
      exit(1);
  }

  ThreadInfo threadInfo;
  char myID = -1;
  
  ConeController cone;
  GunController gun;

  if(strcmp(argv[1], "cone")==0){
    myID = 0;
    cone.Initialize();
    controller = &cone;
  }
  else if(strcmp(argv[1], "gun")==0){
    myID = 01;
    gun.Initialize();
    controller = &gun;
  }
  else if(strcmp(argv[1], "radar")==0){
    myID = 02;
  }
  else if(strcmp(argv[1], "turret")==0){
    myID = 03;
  }
  else{
    //std::cout<<"Controller name not found.( cone gun radar turret )"<<std::endl;
    return 0;
  } 

  GPIOPin p("13");
  p.ExportPin();
  p.SetPinDir("out");
  AudioSystem a;
  GraphicsSystem g;
  NetworkingSystem n(27015, "192.168.77.106");
  //std::cout<<"CONNECTED"<<std::endl;
  std::string hellomsg("+");
  hellomsg += myID;
  int res = n.Send(hellomsg.c_str(), 2);
  //std::cout<<hellomsg[0]<<(int)hellomsg[1]<<std::endl;
  
  g.LoadTextures("../Assets/Textures.JSON");
  
  bool toSend = false;
  char buf[1024] = {0};
  int pos = 0;
  int netResult = 0;
  int state = 0;
  int prevState = 0;
  float deltatime = 0.16f, gDt, rDt,sDt,iDt;
  
  while(true){
    start = clock();
    a.Update(deltatime);
    
    bool updated = false;

    do{
      memset((void*)buf, 0, 1024);
      
      netResult = n.Receive((buf),1023);
      
      pos = 0;
      ProcessResponse(pos, buf, netResult, &g, &n, &a);
    }while(netResult > 0);
    
    g.Draw();
    
    toSend = !toSend;
    inputstream = "~";
    inputstream += controller->GetInputData();
    if(toSend && inputstream.length() > 0){
      std::vector<char> v(inputstream.length() + 1);
      for(unsigned i = 0; i < inputstream.length(); ++i)v[i] = inputstream[i];
      char* pc = &v[0];
      int sentbytes = n.Send(pc, inputstream.length());
      inputstream = "";
    }
    
    deltatime = clock() - start;
    deltatime /= CLOCKS_PER_SEC;
    ////std::cout<<deltatime<<std::endl;
    if(ctrl_c_pressed)
    {
        //std::cout << "Ctrl^C Pressed unexporting pins" <<std::endl;
        controller->Uninitialize();
        break;
    }
  }
  return 0;
}
