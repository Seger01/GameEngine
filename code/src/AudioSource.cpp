#include "AudioSource.h"

AudioSource::AudioSource(std::string aPath, bool aIsMusic)
    : mFileName{aPath}, mPlayOnAwake{false}, mLoop{false}, mVolume{100},
      mXCoord{0}, mXVelocity{0} {
  if (aIsMusic) {
    // addMusic(aPath);
  } else {
    // addSound(aPath);
  }
}