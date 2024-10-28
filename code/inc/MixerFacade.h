#pragma once

#include "IAudioFacade.h"
#include "MixerContainer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class MixerFacade : public IAudioFacade {
private:
  int findAvailableChannel();

private:
  const unsigned mChannelCount{MIX_CHANNELS};
  unsigned mLastUsedChannel; // to help with finding the next available channel
  MixerContainer mMixerContainer;
};