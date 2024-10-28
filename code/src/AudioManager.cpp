#include "AudioManager.h"
#include "MixerFacade.h"

AudioManager::AudioManager() { mFacade = std::make_unique<MixerFacade>(); }

void AudioManager::play(const AudioSource &aSource) {
  // todo
}

void AudioManager::stop(const AudioSource &aSource) {
  // todo
}

void AudioManager::wake() {
  // todo
}

IAudioFacade &AudioManager::getFacade() { return *mFacade.get(); }