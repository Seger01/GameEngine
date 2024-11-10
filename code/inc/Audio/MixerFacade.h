#pragma once

#include "IAudioFacade.h"
#include "MixerContainer.h"
#include <SDL_mixer.h>

class MixerFacade : public IAudioFacade {
public:
    MixerFacade();

private:
    void addSound(std::string aPath) override;
    void playSound(std::string aPath, bool aLooping, unsigned aVolume) override;
    void addMusic(std::string aPath) override;
    void playMusic() override;
    void stopMusic() override;
    int findAvailableChannel();

private:
    const unsigned mChannelCount{MIX_CHANNELS};
    unsigned mLastUsedChannel; // to help with finding the next available channel
    MixerContainer mMixerContainer;
};