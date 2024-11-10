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

void MixerFacade::addSound(std::string aPath) {
    if (mMixerContainer.getSound(aPath) != nullptr) {
        std::cout << "Sound already loaded: " << aPath << std::endl;
        return;
    }
    Mix_Chunk* sound = Mix_LoadWAV(aPath.c_str());
    if (sound == NULL) {
        throw std::runtime_error("Failed to load sound. SDL_Mixer error: " + std::string(Mix_GetError()));
    }
    mMixerContainer.addSound(aPath, *sound);
}

void MixerFacade::playSound(std::string aPath, bool aLooping, unsigned aVolume) {
    Mix_Chunk* sound = mMixerContainer.getSound(aPath);
    if (sound == nullptr) {
        throw std::runtime_error("Sound not found: " + aPath);
    }

    int channel = findAvailableChannel();
    Mix_Volume(channel, aVolume);
    Mix_PlayChannel(channel, sound, aLooping ? -1 : 0);
}

void MixerFacade::addMusic(std::string aPaht) {
    Mix_Music* music = Mix_LoadMUS(aPaht.c_str());
    if (music == NULL) {
        throw std::runtime_error("Failed to load music. SDL_Mixer error: " + std::string(Mix_GetError()));
    }
    mMixerContainer.addMusic(music);
}

void MixerFacade::playMusic() {
    Mix_Music* music = mMixerContainer.getMusic();
    if (music == nullptr) {
        throw std::runtime_error("Music not found.");
    }

    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(music, -1);
    }
}

void MixerFacade::stopMusic() { Mix_HaltMusic(); }

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