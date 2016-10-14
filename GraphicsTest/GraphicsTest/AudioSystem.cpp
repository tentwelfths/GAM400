#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "AudioSystem.h"

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

  //ERRCHECK(system->loadBankFile(("../Assets/Master Bank.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));
  //
  //ERRCHECK(system->loadBankFile(("../Assets/Master Bank.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

  return true;
}
void AudioSystem::Update(double dt)
{

}
void AudioSystem::Shutdown()
{

}

void AudioSystem::RegisterComponent(AudioComponent*comp)
{
  mComponents_.push_back(comp);
}

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
  if (result != FMOD_OK)
  {
    std::cout << "FMOD ERROR: " << result << " FILE " << file << " LINE " << line<<std::endl;
  }
}