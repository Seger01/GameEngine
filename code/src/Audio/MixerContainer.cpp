#include "MixerContainer.h"
#include <stdexcept>

void MixerContainer::addSound(std::string aPath, Mix_Chunk aSound) { mSfx.insert({aPath, aSound}); }

Mix_Chunk* MixerContainer::getSound(std::string aIndex) {
    try {
        return &mSfx.at(aIndex);
    } catch (const std::out_of_range& e) {
        return nullptr;
    }
}

void MixerContainer::addMusic(Mix_Music* aMusic) { mMusic = aMusic; }

Mix_Music* MixerContainer::getMusic() { return mMusic; }

void MixerContainer::clear() {
    mSfx.clear();
    mMusic = nullptr;
}