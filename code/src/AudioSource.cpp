#include "AudioSource.h"
#include "EngineBravo.h"
#include <stdexcept>

AudioSource::AudioSource(std::string aPath, bool aIsMusic)
    : mFileName{aPath}, mPlayOnAwake{false}, mLoop{false},
      mIsMusic(aIsMusic), mVolume{100}, mXCoord{0}, mXVelocity{0} {}

void AudioSource::play(bool aLooping) {
  setLooping(aLooping);

  EngineBravo::getInstance().getAudioManager().play(*this);
}

void AudioSource::stop() {
  if (mIsMusic) {
    EngineBravo::getInstance().getAudioManager().stop(*this);
  } else {
    throw std::logic_error("Only music sources can be stopped.");
  }
}

void AudioSource::setLooping(bool aLooping) { mLoop = aLooping; }