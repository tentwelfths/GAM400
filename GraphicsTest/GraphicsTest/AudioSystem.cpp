#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "AudioSystem.h"
#include "TransformComponent.h"

AudioSystem::AudioSystem()
{
  mName_ = "AudioSystem";
}
bool AudioSystem::Initialize()
{
  ERRCHECK(FMOD::Studio::System::create(&system));

  // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
  //FMOD::System* lowLevelSystem = NULL;
  ERRCHECK(system->getLowLevelSystem(&lowLevelSystem));
  ERRCHECK(lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0));

  ERRCHECK(system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

  ERRCHECK(system->loadBankFile(("../Assets/Master Bank.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
  
  ERRCHECK(system->loadBankFile(("../Assets/Master Bank.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

  CreateEventDescriptions("../Assets/GUIDs.txt");
  FMOD_3D_ATTRIBUTES attributes = { { 0 } };
  attributes.forward.y = 1.0f;
  attributes.up.z = 1.0f;
  ERRCHECK(system->setListenerAttributes(0, &attributes));

  //FMOD::Studio::EventInstance* eventInstance = NULL;
  //ERRCHECK(mSounds["Music"].desc->createInstance(&eventInstance));
  //
  //ERRCHECK(eventInstance->start());
  //// Release will clean up the instance when it completes
  //ERRCHECK(eventInstance->release());


  return true;
}
void AudioSystem::Update(double dt)
{
  for (auto iter : mMessages_){
    switch (iter.type){
    case MessageType::PLAYSOUND:{
      auto * msg = reinterpret_cast<PlaySoundMessage *>(iter.data);
      PlaySoundEffect(msg->name);
      break;
    }
    case MessageType::PLAY3DSOUND:{
      auto * msg = reinterpret_cast<Play3DSoundMessage *>(iter.data);
      
      Play3DSoundEffect(msg->name, msg->source, msg->listener);
      break;
    }
    }
  }
  mMessages_.clear();
  system->update();
}
void AudioSystem::Shutdown()
{

}

void AudioSystem::RegisterComponent(AudioComponent*comp)
{
  mComponents_.push_back(comp);
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

void AudioSystem::Play3DSoundEffect(std::string name, TransformComponent * soundSource, TransformComponent * listener)
{
  FMOD::Studio::EventInstance* eventInstance = NULL;
  ERRCHECK(mSounds[name].desc->createInstance(&eventInstance));
  FMOD_3D_ATTRIBUTES attributes = { { 0 } };
  attributes.forward.z = 1.0f;
  attributes.up.y = 1.0f;
  attributes.position.x = listener->mPositionX() - soundSource->mPositionX();
  attributes.position.y = listener->mPositionY() - soundSource->mPositionY();
  attributes.position.z = 0;
  eventInstance->set3DAttributes(&attributes);
  ERRCHECK(eventInstance->start());
  // Release will clean up the instance when it completes
  ERRCHECK(eventInstance->release());
}