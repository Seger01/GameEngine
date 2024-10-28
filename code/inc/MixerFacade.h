#pragma once

#include "IAudioFacade.h"
#include "MixerContainer.h"

class MixerFacade : public IAudioFacade {
private:
  int findAvailableChannel();

private:
  const unsigned mChannelCount;
  unsigned mLastUsedChannel; // to help with finding the next available channel
  MixerContainer mMixerContainer;
};