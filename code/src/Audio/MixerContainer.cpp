#include "MixerContainer.h"
#include <stdexcept>

MixerContainer::MixerContainer() : mMusic(nullptr) {}

MixerContainer::~MixerContainer() { clear(); }

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

MixerContainer::MixerContainer(MixerContainer&& aOther)
{
	mSfx = std::move(aOther.mSfx);
	mMusicPath = std::move(aOther.mMusicPath);
	mMusic = aOther.mMusic;
	aOther.mMusic = nullptr;
}

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
void MixerContainer::addSound(std::string aPath, Mix_Chunk* aSound) { mSfx.insert({aPath, aSound}); }

Mix_Chunk* MixerContainer::getSound(std::string aIndex)
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

const Mix_Chunk* MixerContainer::getSound(std::string aIndex) const
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

void MixerContainer::clear()
{
	// Stop all channels from playing
	Mix_HaltChannel(-1);
	for (auto& sound : mSfx)
	{
		Mix_FreeChunk(sound.second);
		// sound.second = nullptr;
	}
	Mix_FreeMusic(mMusic);

	mSfx.clear();
	mMusic = nullptr;
}