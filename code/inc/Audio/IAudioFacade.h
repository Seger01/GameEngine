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
 * @brief This class is an interface for an audio library, acting as a facade for the audio system. It helps to abstract
 * the audio system from the rest of the engine.
 */
class IAudioFacade
{
public:
	/**
	 * @brief Load a sound effect into memory. If the sound is already loaded, do nothing.
	 *
	 * @param aPath The path to the sound effect. Must be an absolute path.
	 */
	virtual void loadSound(const std::string& aPath) = 0;
	/**
	 * @brief Load a music file into memory. If the music is already loaded, it is overwritten
	 *
	 * @param aPath The path to the music file. Must be an absolute path.
	 *
	 * @throw std::runtime_error if the music file could not be loaded
	 */
	virtual void loadMusic(const std::string& aPath) = 0;
	/**
	 * @brief Unload all sounds and music from memory
	 */
	virtual void unloadAll() = 0;
	/**
	 * @brief Check if a sound effect is loaded
	 *
	 * @param aPath The path to the sound effect. Must be relative to resources folder.
	 * @return true if the sound effect is loaded
	 */
	virtual bool audioIsLoaded(const std::string& aPath) const = 0;
	/**
	 * @brief Check if any music is loaded
	 *
	 * @return true if music is loaded. False if not.
	 */
	virtual bool musicIsLoaded() const = 0;

	/**
	 * @brief Play a sound effect
	 *
	 * @param aPath The path to the sound effect. Must be an absolute path.
	 * @param aLooping Whether the sound should loop
	 * @param aVolume The volume of the sound.
	 * @param aDirection The direction of the sound. Negative is left, positive is right, 0 is center.
	 */
	virtual void playSound(const std::string& aPath, bool aLooping, unsigned aVolume, int aDirection) = 0;
	/**
	 * @brief Play the music. If the music is already playing, do nothing.
	 *
	 * @param aVolume The volume of the music. If it is below zero, volume is not adjusted (and will be the same as it
	 * was last set.)
	 */
	virtual void playMusic(int aVolume) = 0;
	/**
	 * @brief Pause the music. If no music is playing, do nothing.
	 */
	virtual void pauseMusic() = 0;
	/**
	 * @brief Resume the music. If no music is paused, start playing the music from the beginning.
	 */
	virtual void resumeMusic() = 0;
	/**
	 * @brief Stop the music. If no music is playing, do nothing. Calling playMusic after this will start the music from
	 * the beginning.
	 */
	virtual void stopMusic() = 0;
	/**
	 * @brief Check if a sound effect is playing
	 *
	 * @param aPath The path to the sound effect. Must be relative to the resources folder.
	 *
	 * @return true if the sound effect is playing. False otherwise.
	 */
	virtual bool isPlaying(const std::string& aPath) const = 0;
	/**
	 * @brief Check if music is playing
	 *
	 * @return true if any music is playing. False otherwise.
	 */
	virtual bool isMusicPlaying() const = 0;
};