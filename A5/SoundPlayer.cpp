
#include "SoundPlayer.hpp"
#include <iostream>

SoundPlayer::SoundPlayer() {
}

void SoundPlayer::Init() {
  this->engine = createIrrKlangDevice();
  if (this->engine == nullptr) {
    std::cout << "Engine failed to start" << std::endl;
  }
}

SoundPlayer::~SoundPlayer() {
  if (!this->engine) return;
  this->engine->drop();
}

void SoundPlayer::PlayLoopedSound(std::string path, float volume) {
  if (!this->engine) return;
  auto music = this->engine->play2D(path.c_str(), true, false, true);
  if (music != nullptr) {
    music->setVolume(volume);
  }
}

void SoundPlayer::PlaySoundOneShot(std::string path, float volume) {
  if (!this->engine) return;
  auto music = this->engine->play2D(path.c_str(), false, false, true);
  if (music != nullptr) {
    music->setVolume(volume);
    this->sounds.emplace_back(music);
  }
}

void SoundPlayer::StopSound() {
  if (!this->engine) return;

  for (ISound * sound : this->sounds) {
    if (sound != nullptr) {
      sound->stop();
    }
  }

  this->sounds.clear();
}