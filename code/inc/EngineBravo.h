#pragma once

#include "AudioManager.h"

class EngineBravo {
public:
  static EngineBravo &getInstance();

  AudioManager &getAudioManager();

private:
  EngineBravo() = default;  // Private constructor
  ~EngineBravo() = default; // Destructor

  // Delete copy constructor and assignment operator
  EngineBravo(const EngineBravo &) = delete;
  EngineBravo &operator=(const EngineBravo &) = delete;

  AudioManager mAudioManager;
};