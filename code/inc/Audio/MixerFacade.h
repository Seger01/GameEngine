/**
 * @file MixerFacade.h
 *
 * @brief This file contains the declaration of the MixerFacade class
 */
#pragma once

#include "IAudioFacade.h"
#include "MixerContainer.h"
#include <SDL_mixer.h>

/**
 * @class MixerFacade
 *
 * @brief This class implements the IAudioFacade interface, and is responsible for interacting with the SDL mixer
 * library, and performing all audio operations (such as playing, stopping and loading). This class is also responsible
 * for managing which track (in SDL known as channel) to play audio on (because this is not handled by SLD mixer).
 */
class MixerFacade : public IAudioFacade
{
public:
	MixerFacade();

public:
	void loadSound(const std::string& aPath) override;
	void loadMusic(const std::string& aPath) override;
	void unloadAll() override;
	bool audioIsLoaded(const std::string& aPath) const override;
	bool musicIsLoaded() const override;

	void playSound(const std::string& aPath, bool aLooping, unsigned aVolume, int aDirection) override;
	void playMusic(int aVolume) override;
	void pauseMusic() override;
	void resumeMusic() override;
	void stopMusic() override;

	bool isPlaying(const std::string& aPath) const override;
	bool isMusicPlaying() const override;

	int distanceToAngle(int aDirection) const;
	int findAvailableChannel();

private:
	/// @brief The number of channels to use. Defaults to 8.
	const unsigned mChannelCount;
	/// @brief The last channel on which a sound effect was played. To help with finding the next available channel
	unsigned mLastUsedChannel;
	/// @brief The container for all sound effects and music
	MixerContainer mMixerContainer;
};