#include "MixerFacade.h"
#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdexcept>

MixerFacade::MixerFacade() {
    // Initialize SDL with audio support
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void MixerFacade::loadSound(const std::string& aPath) {
    // Check if the sound is already loaded
    if (audioIsLoaded(aPath)) {
        std::cout << "Sound already loaded: " << aPath << std::endl;
        return;
    }
    // Load the sound
    Mix_Chunk* sound = Mix_LoadWAV(aPath.c_str());
    if (sound == NULL) {
        throw std::runtime_error("Failed to load sound. SDL_Mixer error: " + std::string(Mix_GetError()));
    }
    mMixerContainer.addSound(aPath, *sound);
}

void MixerFacade::loadMusic(const std::string& aPath) {
    Mix_Music* music = Mix_LoadMUS(aPath.c_str());
    if (music == NULL) {
        throw std::runtime_error("Failed to load music. SDL_Mixer error: " + std::string(Mix_GetError()));
    }
    mMixerContainer.addMusic(music);
}

void MixerFacade::unloadAll() { mMixerContainer.clear(); }

bool MixerFacade::audioIsLoaded(const std::string& aPath) const { return mMixerContainer.getSound(aPath) != nullptr; }

bool MixerFacade::musicIsLoaded() const { return mMixerContainer.getMusic() != nullptr; }

void MixerFacade::playSound(std::string aPath, bool aLooping, unsigned aVolume, int aDirection) {
    Mix_Chunk* sound = mMixerContainer.getSound(aPath);
    if (sound == nullptr) {
        throw std::runtime_error("Sound not found: " + aPath);
    }

    int channel = findAvailableChannel();
    Mix_Volume(channel, aVolume);

    Mix_SetPosition(channel, distanceToAngle(aDirection), aDirection);
    Mix_PlayChannel(channel, sound, aLooping ? -1 : 0);
}

void MixerFacade::playMusic(int aVolume) {
    Mix_Music* music = mMixerContainer.getMusic();
    if (music == nullptr) {
        throw std::runtime_error("Music not found.");
    }

    Mix_VolumeMusic(aVolume);
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(music, -1);
    }
}

void MixerFacade::stopMusic() { Mix_HaltMusic(); }

bool MixerFacade::isPlaying(const std::string& aPath) const {
    const Mix_Chunk* chunk = mMixerContainer.getSound(aPath);
    int numChannels = Mix_AllocateChannels(-1); // Get the number of allocated channels
    for (int i = 0; i < numChannels; ++i) {
        if (Mix_Playing(i) && Mix_GetChunk(i) == chunk) {
            return true; // The chunk is playing on this channel
        }
    }
    return false; // The chunk is not playing on any channel
}

int MixerFacade::distanceToAngle(int aDirection) const {
    if (aDirection < 0) {
        return 90;
    }
    if (aDirection > 0) {
        return 270;
    }
    return 0;
}

int MixerFacade::findAvailableChannel() {
    for (int i = 0; i < mChannelCount; i++) {
        if (!Mix_Playing(i)) {
            mLastUsedChannel = i;
            return i;
        }
    }

    int nextChannel = mLastUsedChannel + 1;
    nextChannel = nextChannel % mChannelCount;
    mLastUsedChannel = nextChannel;
    return nextChannel;
}