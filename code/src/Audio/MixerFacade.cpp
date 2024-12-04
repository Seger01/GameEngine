#include "MixerFacade.h"
#include "FSConverter.h"
#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdexcept>

MixerFacade::MixerFacade() : mChannelCount(MIX_CHANNELS), mLastUsedChannel(0)
{
	// Initialize SDL with audio support
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
	}
}

/**
 * @brief Load a sound effect into the mixer container. If the sound is already loaded, do nothing.
 *
 * @param aPath The path to the sound effect. Must be an absolute path.
 */
void MixerFacade::loadSound(const std::string& aPath)
{
	// Check if the sound is already loaded
	if (audioIsLoaded(aPath))
	{
		std::cout << "Sound already loaded: " << aPath << std::endl;
		return;
	}
	// Load the sound
	Mix_Chunk* sound = Mix_LoadWAV(aPath.c_str());
	if (sound == nullptr)
	{
		throw std::runtime_error("Failed to load sound. SDL_Mixer error: " + std::string(Mix_GetError()));
	}
	mMixerContainer.addSound(aPath, sound);
}

/**
 * @brief Load a music file into the mixer container. If the music is already loaded, it is overwritten
 */
void MixerFacade::loadMusic(const std::string& aPath)
{
	Mix_Music* music = Mix_LoadMUS(aPath.c_str());
	if (music == nullptr)
	{
		throw std::runtime_error("Failed to load music. SDL_Mixer error: " + std::string(Mix_GetError()));
	}
	mMixerContainer.addMusic(aPath, music);
}

/**
 * @brief Unload all sounds and music from the mixer container
 */
void MixerFacade::unloadAll() { mMixerContainer.clear(); }

/**
 * @brief Check if a sound effect is loaded
 *
 * @param aPath The path to the sound effect. Must relative to resources folder.
 * @return true if the sound effect is loaded
 */
bool MixerFacade::audioIsLoaded(const std::string& aPath) const
{
	std::string wholePath = FSConverter().getResourcePath(aPath);
	return mMixerContainer.getSound(wholePath) != nullptr;
}

/**
 * @brief Check if any music is loaded
 */
bool MixerFacade::musicIsLoaded() const { return mMixerContainer.getMusic() != nullptr; }

/**
 * @brief Play a sound effect
 *
 * @param aPath The path to the sound effect. Must be an absolute path.
 * @param aLooping Whether the sound should loop
 * @param aVolume The volume of the sound.
 * @param aDirection The direction of the sound. Negative is left, positive is right, 0 is center.
 */
void MixerFacade::playSound(const std::string& aPath, bool aLooping, unsigned aVolume, int aDirection)
{
	// Pointer, because sdl mixer does not work with references
	Mix_Chunk* sound = mMixerContainer.getSound(aPath);
	if (sound == nullptr)
	{
		throw std::runtime_error("Sound not found: " + aPath);
	}

	int channel = findAvailableChannel();
	Mix_Volume(channel, aVolume);

	Mix_SetPosition(channel, distanceToAngle(aDirection), std::abs(aDirection));
	Mix_PlayChannel(channel, sound, aLooping ? -1 : 0);
}

/**
 * @brief Play the music. If the music is already playing, do nothing.
 *
 * @param aVolume The volume of the music
 */
void MixerFacade::playMusic(int aVolume)
{
	Mix_Music* music = mMixerContainer.getMusic();
	if (music == nullptr)
	{
		throw std::runtime_error("Music not found.");
	}

	Mix_VolumeMusic(aVolume);
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(music, -1);
	}
}

void MixerFacade::pauseMusic() { Mix_PauseMusic(); }

void MixerFacade::resumeMusic() { Mix_ResumeMusic(); }

void MixerFacade::stopMusic() { Mix_HaltMusic(); }

/**
 * @brief Check if a sound effect is playing
 *
 * @param aPath The path to the sound effect. Must be relative to the resources folder.
 */
bool MixerFacade::isPlaying(const std::string& aPath) const
{
	std::string wholePath = FSConverter().getResourcePath(aPath);
	const Mix_Chunk* chunk = mMixerContainer.getSound(wholePath);
	int numChannels = Mix_AllocateChannels(-1); // Get the number of allocated channels
	for (int i = 0; i < numChannels; ++i)
	{
		if (Mix_Playing(i) && Mix_GetChunk(i) == chunk)
		{
			return true; // The chunk is playing on this channel
		}
	}
	return false; // The chunk is not playing on any channel
}

bool MixerFacade::isMusicPlaying() const { return Mix_PlayingMusic(); }

/**
 * @brief Convert a direction to an angle. The greater the argument, the further away the sound. This is simulated by
 * increasing the angle, up to a maximum of 90 degrees for the right, and -90 degrees for the left.
 *
 * @param aDirection The direction to convert. Negative is left, positive is right, 0 is center.
 *
 * @return The angle of the direction.
 */
int MixerFacade::distanceToAngle(int aDirection) const
{
	if (aDirection >= 0)
	{
		// Play to the right
		return std::min(aDirection, 90);
	}
	// Play to the left
	return std::max(aDirection, -90);
}

/**
 * @brief Find the next available channel to play a sound effect
 *
 * If all channels are playing, the next channel is the last used channel + 1.
 * If the last used channel is the last channel, the next channel is the first channel.
 * Sets the last used channel to whichever channel is returned.
 *
 * @return The channel number
 */
int MixerFacade::findAvailableChannel()
{
	for (int i = 0; i < mChannelCount; i++)
	{
		if (!Mix_Playing(i))
		{
			mLastUsedChannel = i;
			return i;
		}
	}

	int nextChannel = mLastUsedChannel + 1;
	nextChannel = nextChannel % mChannelCount;
	mLastUsedChannel = nextChannel;
	return nextChannel;
}