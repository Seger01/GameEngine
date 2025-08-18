/**
 * @file MixerContainer.cpp
 *
 * @brief Implementation of MixerContainer using SoLoud
 */
#include "Audio/MixerContainer.h"
#include <stdexcept>

/**
 * @brief Construct a new MixerContainer::MixerContainer object
 */
MixerContainer::MixerContainer() : mMusic(nullptr) {}

/**
 * @brief Destructor: Free all resources
 */
MixerContainer::~MixerContainer() { clear(); }

/**
 * @brief Copy constructor
 */
MixerContainer::MixerContainer(const MixerContainer& aOther)
{
	// Copy the sound effects
	for (auto& sound : aOther.mSfx)
	{
		SoLoud::Wav* newWav = new SoLoud::Wav();
		newWav->load(sound.first.c_str());
		mSfx[sound.first] = newWav;
	}
	// Copy the music
	if (!aOther.mMusicPath.empty())
	{
		mMusic = new SoLoud::WavStream();
		mMusic->load(aOther.mMusicPath.c_str());
		mMusicPath = aOther.mMusicPath;
	}
	else
	{
		mMusic = nullptr;
	}
}

/**
 * @brief Move constructor
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
 */
MixerContainer& MixerContainer::operator=(const MixerContainer& aOther)
{
	if (this != &aOther)
	{
		clear();
		// Copy sound effects
		for (auto& sound : aOther.mSfx)
		{
			SoLoud::Wav* newWav = new SoLoud::Wav();
			newWav->load(sound.first.c_str());
			mSfx[sound.first] = newWav;
		}
		// Copy music
		if (!aOther.mMusicPath.empty())
		{
			mMusic = new SoLoud::WavStream();
			mMusic->load(aOther.mMusicPath.c_str());
			mMusicPath = aOther.mMusicPath;
		}
		else
		{
			mMusic = nullptr;
			mMusicPath.clear();
		}
	}
	return *this;
}

/**
 * @brief Move assignment operator
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
 */
void MixerContainer::addSound(const std::string& aPath, SoLoud::Wav* aSound) { mSfx.insert({aPath, aSound}); }

/**
 * @brief Get a sound effect from the container. If not found, nullptr.
 */
SoLoud::Wav* MixerContainer::getSound(const std::string& aIndex)
{
	auto it = mSfx.find(aIndex);
	return (it != mSfx.end()) ? it->second : nullptr;
}

const SoLoud::Wav* MixerContainer::getSound(const std::string& aIndex) const
{
	auto it = mSfx.find(aIndex);
	return (it != mSfx.end()) ? it->second : nullptr;
}

/**
 * @brief Add a music file to the container
 */
void MixerContainer::addMusic(const std::string& aPath, SoLoud::WavStream* aMusic)
{
	if (mMusic)
	{
		delete mMusic;
	}
	mMusic = aMusic;
	mMusicPath = aPath;
}

SoLoud::WavStream* MixerContainer::getMusic() { return mMusic; }

const SoLoud::WavStream* MixerContainer::getMusic() const { return mMusic; }

/**
 * @brief Clear all sound effects and music from the container
 */
void MixerContainer::clear()
{
	for (auto& sound : mSfx)
	{
		delete sound.second;
	}
	mSfx.clear();
	if (mMusic)
	{
		delete mMusic;
		mMusic = nullptr;
	}
	mMusicPath.clear();
}
