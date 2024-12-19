/**
 * @file MixerContainer.cpp
 *
 * @brief This file contains the implementation of the MixerContainer class
 */
#include "MixerContainer.h"
#include <stdexcept>

/**
 * @brief Construct a new MixerContainer::MixerContainer object. Initializes mMusic to nullptr
 */
MixerContainer::MixerContainer() : mMusic(nullptr) {}

/**
 * @brief Destroy the MixerContainer::MixerContainer object. Calls clear to free all memory
 */
MixerContainer::~MixerContainer() { clear(); }

/**
 * @brief Construct a new MixerContainer::MixerContainer object. Copy constructor
 *
 * @param aOther The MixerContainer to copy
 */
MixerContainer::MixerContainer(const MixerContainer& aOther)
{
	// Copy the sound effects
	for (auto& sound : aOther.mSfx)
	{
		mSfx[sound.first] = Mix_LoadWAV(sound.first.c_str());
	}
	// Copy the music
	mMusic = Mix_LoadMUS(aOther.mMusicPath.c_str());
}

/**
 * @brief Construct a new MixerContainer::MixerContainer object. Move constructor
 *
 * @param aOther The MixerContainer to move
 */
MixerContainer::MixerContainer(MixerContainer&& aOther)
{
	mSfx = std::move(aOther.mSfx);
	mMusicPath = std::move(aOther.mMusicPath);
	mMusic = aOther.mMusic;
	aOther.mMusic = nullptr;
}

/**
 * @brief Copy assignment operator
 *
 * @param aOther The MixerContainer to copy
 * @return MixerContainer& The newly copied MixerContainer
 */
MixerContainer& MixerContainer::operator=(const MixerContainer& aOther)
{
	if (this != &aOther)
	{
		clear();
		// Copy the sound effects
		for (auto& sound : aOther.mSfx)
		{
			mSfx[sound.first] = Mix_LoadWAV(sound.first.c_str());
		}
		// Copy the music
		mMusic = Mix_LoadMUS(aOther.mMusicPath.c_str());
	}
	return *this;
}

/**
 * @brief Move assignment operator
 *
 * @param aOther The MixerContainer to move
 * @return MixerContainer& The newly moved MixerContainer
 */
MixerContainer& MixerContainer::operator=(MixerContainer&& aOther)
{
	if (this != &aOther)
	{
		clear();
		mSfx = std::move(aOther.mSfx);
		mMusicPath = std::move(aOther.mMusicPath);
		mMusic = aOther.mMusic;
		aOther.mMusic = nullptr;
	}
	return *this;
}

/**
 * @brief Add a sound effect to the container
 *
 * @note Does not check if the sound is already in the container
 */
void MixerContainer::addSound(const std::string& aPath, Mix_Chunk* aSound) { mSfx.insert({aPath, aSound}); }

/**
 * @brief Get a sound effect from the container. If the effect is not found, return nullptr
 */
Mix_Chunk* MixerContainer::getSound(const std::string& aIndex)
{
	try
	{
		return mSfx.at(aIndex);
	}
	catch (const std::out_of_range& e)
	{
		return nullptr;
	}
}

/**
 * @brief Get a sound effect from the container. If the effect is not found, return nullptr
 */
const Mix_Chunk* MixerContainer::getSound(const std::string& aIndex) const
{
	try
	{
		return mSfx.at(aIndex);
	}
	catch (const std::out_of_range& e)
	{
		return nullptr;
	}
}

/**
 * @brief Add a music file to the container
 *
 * @note Does not check if the music is already in the container
 */
void MixerContainer::addMusic(const std::string& aPath, Mix_Music* aMusic)
{
	mMusic = aMusic;
	mMusicPath = aPath;
}

Mix_Music* MixerContainer::getMusic() { return mMusic; }

const Mix_Music* MixerContainer::getMusic() const { return mMusic; }

/**
 * @brief Clear all sound effects and music from the container. Stop all channels from playing.
 */
void MixerContainer::clear()
{
	// Stop all channels from playing
	Mix_HaltChannel(-1);
	for (auto& sound : mSfx)
	{
		Mix_FreeChunk(sound.second);
	}
	Mix_FreeMusic(mMusic);

	mSfx.clear();
	mMusic = nullptr;
}