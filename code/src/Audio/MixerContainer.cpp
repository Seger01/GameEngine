#include "MixerContainer.h"
#include <stdexcept>

MixerContainer::~MixerContainer() { clear(); }

void MixerContainer::addSound(std::string aPath, Mix_Chunk aSound) { mSfx.insert({aPath, aSound}); }

Mix_Chunk* MixerContainer::getSound(std::string aIndex) {
    try {
        return &mSfx.at(aIndex);
    } catch (const std::out_of_range& e) {
        return nullptr;
    }
}

const Mix_Chunk* MixerContainer::getSound(std::string aIndex) const {
    try {
        return &mSfx.at(aIndex);
    } catch (const std::out_of_range& e) {
        return nullptr;
    }
}

void MixerContainer::addMusic(Mix_Music* aMusic) { mMusic = aMusic; }

Mix_Music* MixerContainer::getMusic() { return mMusic; }

void MixerContainer::clear() {
    for (auto& sound : mSfx) {
        Mix_FreeChunk(&sound.second);
        // sound.second = nullptr;
    }
    Mix_FreeMusic(mMusic);

    mSfx.clear();
    mMusic = nullptr;
}