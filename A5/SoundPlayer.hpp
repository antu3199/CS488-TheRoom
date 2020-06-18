#pragma once

#include "irrklang/irrKlang.h"
#include <string>
#include <vector>

using namespace irrklang;

class SoundPlayer {
  public:
  SoundPlayer();
  ~SoundPlayer();

  void Init();

  void PlayLoopedSound(std::string path, float volume);
  void PlaySoundOneShot(std::string path, float volume);
  void StopSound();
  
  private:
    ISoundEngine * engine;
    std::vector<ISound*> sounds;
};

