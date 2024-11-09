#include "MixerContainer.h"

void MixerContainer::addSound(std::string aPath, Mix_Chunk aSound) { mSfx.insert({aPath, aSound}); }

Mix_Chunk* MixerContainer::getSound(std::string aIndex) { return &mSfx.at(aIndex); }

void MixerContainer::addMusic(Mix_Music* aMusic) { mMusic = aMusic; }

Mix_Music* MixerContainer::getMusic() { return mMusic; }

void MixerContainer::clear() {
    mSfx.clear();
    mMusic = nullptr;
}