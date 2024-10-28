#pragma once

#include "AudioManager.h"

class EngineBravo {
  AudioManager &getAudioManager();

private:
  AudioManager mAudioManager;
};