#include "MixerContainer.h"
#include <stdexcept>

MixerContainer::~MixerContainer() { clear(); }

/**
 * @brief Add a sound effect to the container
 *
 * @note Does not check if the sound is already in the container
 */
void MixerContainer::addSound(std::string aPath, Mix_Chunk aSound) { mSfx.insert({aPath, aSound}); }

Mix_Chunk* MixerContainer::getSound(std::string aIndex) {
    try {
        return &mSfx.at(aIndex);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Sound not found: " + aIndex);
    }
}

const Mix_Chunk* MixerContainer::getSound(std::string aIndex) const {
    try {
        return &mSfx.at(aIndex);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Sound not found: " + aIndex);
    }
}

/**
 * @brief Add a music file to the container
 *
 * @note Does not check if the music is already in the container
 */
void MixerContainer::addMusic(Mix_Music* aMusic) { mMusic = aMusic; }

Mix_Music* MixerContainer::getMusic() { return mMusic; }

const Mix_Music* MixerContainer::getMusic() const { return mMusic; }

void MixerContainer::clear() {
    for (auto& sound : mSfx) {
        Mix_FreeChunk(&sound.second);
        // sound.second = nullptr;
    }
    Mix_FreeMusic(mMusic);

    mSfx.clear();
    mMusic = nullptr;
}