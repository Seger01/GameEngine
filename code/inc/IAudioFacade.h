#pragma once

#include <string>

class IAudioFacade {
public:
  void addSound(std::string aPath);
  void playSound(std::string aName, bool aLooping, unsigned aVolume);
  void addMusic(std::string aPath);
  void playMusic();
  void stopMusic();
};