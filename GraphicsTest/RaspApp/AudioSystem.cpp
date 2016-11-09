
#include "AudioSystem.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

#include <fmod.hpp>
#include <fmod_studio.hpp>

AudioSystem::AudioSystem()
{

  ERRCHECK(FMOD::Studio::System::create(&system));

  // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
  //FMOD::System* lowLevelSystem = NULL;
  ERRCHECK(system->getLowLevelSystem(&lowLevelSystem));
  //lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_ALSA);
  int drivers = 0;
  lowLevelSystem->getNumDrivers(&drivers);
  std::cout<<drivers<<std::endl;
  FMOD_GUID guids[11];
  for(int i = 0; i < drivers; ++i){
    char name[128];
    int systemrate, speakermodechannels;
    lowLevelSystem->getDriverInfo(i, name, 128, &guids[i], &systemrate, NULL, &speakermodechannels);
    std::cout<<i<<" " <<name<<std::endl;
  }
  lowLevelSystem->setDriver(2);
  ERRCHECK(lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0));
  ERRCHECK(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

  ERRCHECK(system->loadBankFile(("../Assets/Master Bank.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
  
  ERRCHECK(system->loadBankFile(("../Assets/Master Bank.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

  CreateEventDescriptions("../Assets/GUIDs.txt");
  FMOD_3D_ATTRIBUTES attributes = { { 0 } };
  attributes.forward.y = 1.0f;
  attributes.up.z = 1.0f;
  ERRCHECK(system->setListenerAttributes(0, &attributes));

  FMOD::Studio::EventInstance* eventInstance = NULL;
  ERRCHECK(mSounds["Music"].desc->createInstance(&eventInstance));
  
  ERRCHECK(eventInstance->start());
  // Release will clean up the instance when it completes
  ERRCHECK(eventInstance->release());

}
void AudioSystem::Update(double dt)
{


  system->update();
}
void AudioSystem::Shutdown()
{
    lowLevelSystem->close();
    lowLevelSystem->release();
    system->release();
}

void ERRCHECK_fn(int result, const char *file, int line)
{
  if (result != FMOD_OK)
  {
    std::cout << "FMOD ERROR: " << result << " FILE " << file << " LINE " << line<<std::endl;
  }
}

void AudioSystem::CreateEventDescriptions(std::string filename)
{
  std::ifstream file;
  file.open(filename);
  if (file.is_open() == false)return;
  while (file.eof() == false)
  {
    std::string line = "";
    std::getline(file, line);
    line = line.substr(line.find_first_of(' ')+1);
    if (line.find("event:/") == std::string::npos){
      continue;
    }
    FMOD::Studio::EventDescription * temp;
    ERRCHECK(system->getEvent(line.c_str(), &temp));
    line = line.substr(line.find("event:/") + 7);
    mSounds.insert({line, SoundInstance(temp)});
  }
}

void AudioSystem::PlaySoundEffect(std::string name)
{
  FMOD::Studio::EventInstance* eventInstance = NULL;
  ERRCHECK(mSounds[name].desc->createInstance(&eventInstance));

  ERRCHECK(eventInstance->start());
  // Release will clean up the instance when it completes
  ERRCHECK(eventInstance->release());
}

void AudioSystem::Play3DSoundEffect(std::string name, float x1, float y1, float x2, float y2)
{
  FMOD::Studio::EventInstance* eventInstance = NULL;
  ERRCHECK(mSounds[name].desc->createInstance(&eventInstance));
  FMOD_3D_ATTRIBUTES attributes = { { 0 } };
  attributes.forward.z = 1.0f;
  attributes.up.y = 1.0f;
  attributes.position.x = x2 - x1;
  attributes.position.y = y2 - y1;
  attributes.position.z = 0;
  eventInstance->set3DAttributes(&attributes);
  ERRCHECK(eventInstance->start());
  // Release will clean up the instance when it completes
  ERRCHECK(eventInstance->release());
}