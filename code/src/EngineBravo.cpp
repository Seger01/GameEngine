#include "EngineBravo.h"

EngineBravo &EngineBravo::getInstance() {
  static EngineBravo instance;
  return instance;
}

AudioManager &EngineBravo::getAudioManager() { return mAudioManager; }