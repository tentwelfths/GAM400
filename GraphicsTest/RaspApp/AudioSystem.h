#pragma once
#include <unordered_map>
#include <string>
#include <iostream>

#include <fmod.hpp>
#include <fmod_studio.hpp>

struct SoundInstance{
  SoundInstance(FMOD::Studio::EventDescription * d):desc(d){}
  SoundInstance(const SoundInstance& s) : desc(s.desc){}
  SoundInstance(){
    int b = 0;
  }
  FMOD::Studio::EventDescription * desc;
};

void ERRCHECK_fn(int result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class AudioSystem
{
public:
  AudioSystem();
  virtual void Update(double dt);
  virtual void Shutdown();
  void PlaySoundEffect(std::string name);
  void Play3DSoundEffect(std::string name, float x1, float y1, float x2, float y2);
private:

  void CreateEventDescriptions(std::string filename);

  FMOD::Studio::System * system;
  FMOD::System * lowLevelSystem;
  FMOD::Studio::Bank* masterBank;
  FMOD::Studio::Bank* stringsBank;
  std::unordered_map<std::string, SoundInstance> mSounds;
};