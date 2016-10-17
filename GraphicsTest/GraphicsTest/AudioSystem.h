#pragma once
#include "Standard.h"
#include "System.h"

namespace FMOD{
  class System;
  namespace Studio{
    class System;
    class Bank;
    class EventDescription;
    class EventInstance;
  }
}

struct SoundInstance{
  SoundInstance(FMOD::Studio::EventDescription * d):desc(d){}
  SoundInstance(const SoundInstance& s) : desc(s.desc){}
  SoundInstance(){
    int b = 0;
  }
  FMOD::Studio::EventDescription * desc;
};


class AudioComponent;
class TransformComponent;


void ERRCHECK_fn(int result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class AudioSystem : public System
{
public:
  AudioSystem();
  virtual void RegisterComponent(AudioComponent*);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
  void PlaySoundEffect(std::string name);
  void Play3DSoundEffect(std::string name, TransformComponent * soundSource, TransformComponent * listener);
private:

  void CreateEventDescriptions(std::string filename);
  std::vector<AudioComponent*> mComponents_;

  FMOD::Studio::System * system;
  FMOD::System * lowLevelSystem;
  FMOD::Studio::Bank* masterBank;
  FMOD::Studio::Bank* stringsBank;
  std::unordered_map<std::string, SoundInstance> mSounds;
};