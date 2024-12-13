/**
 * @file IAudioFacade.h
 *
 * @brief This file contains the declaration of the IAudioFacade interface
 */
#pragma once

#include <string>

/**
 * @class IAudioFacade
 *
 * @brief This class is an interface for an audio library, acting as a facade for the audio system
 */
class IAudioFacade
{
public:
	virtual void loadSound(const std::string& aPath) = 0;
	virtual void loadMusic(const std::string& aPath) = 0;
	virtual void unloadAll() = 0;
	virtual bool audioIsLoaded(const std::string& aPath) const = 0;
	virtual bool musicIsLoaded() const = 0;

	virtual void playSound(const std::string& aPath, bool aLooping, unsigned aVolume, int aDirection) = 0;
	virtual void playMusic(int aVolume) = 0;
	virtual void pauseMusic() = 0;
	virtual void resumeMusic() = 0;
	virtual void stopMusic() = 0;

	virtual bool isPlaying(const std::string& aPath) const = 0;
	virtual bool isMusicPlaying() const = 0;
};