#pragma once
#include "Standard.h"
#include "System.h"

namespace FMOD{
  class System;
  namespace Studio{
    class System;
    class Bank;
  }
}

class AudioComponent;


void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class AudioSystem : public System
{
public:
  AudioSystem();
  virtual void RegisterComponent(AudioComponent*);
  virtual bool Initialize();
  virtual void Update(double dt);
  virtual void Shutdown();
private:
  std::vector<AudioComponent*> mComponents_;

  FMOD::Studio::System * system;
  FMOD::System * lowLevelSystem;
  FMOD::Studio::Bank* masterBank;
  FMOD::Studio::Bank* stringsBank;
};