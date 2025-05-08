/**
 * @file AudioManager.cpp
 *
 * @brief This file contains the implementation of the AudioManager class
 */
#include "Audio/AudioManager.h"
#include "Audio/IAudioFacade.h"
#include "Audio/MixerFacade.h"
#include "Components/AudioSource.h"
#include "GameObject.h"
#include <stdexcept>

/**
 * @brief AudioManger constructor. Currently creates a MixerFacade object, as it is the only audio library facade
 * currently available.
 */
AudioManager::AudioManager() { mFacade = std::make_unique<MixerFacade>(); }

/**
 * @brief Play the audio source.
 *
 * @param aSource The audio source to play. Uses the file name, looping, volume, and x direction.
 */
void AudioManager::play(const AudioSource& aSource)
{
	if (aSource.isMusic())
	{
		// If the music is not loaded, load it
		if (!mFacade->musicIsLoaded())
		{
			mFacade->loadMusic(aSource.getFileName());
		}
		mFacade->playMusic(aSource.getVolume());
	}
	else
	{
		// If the sound is not loaded, load it
		if (!mFacade->audioIsLoaded(aSource.getFileName()))
		{
			mFacade->loadSound(aSource.getFileName());
		}
		mFacade->playSound(aSource.getFileName(), aSource.getLooping(), aSource.getVolume(), aSource.getXDirection());
	}
}

/**
 * @brief Pause the given audio source (must be music). If the music is not playing, nothing happens.
 *
 * @param aSource The audio source to pause.
 *
 * @throw std::logic_error if the audio source is not music
 */
void AudioManager::pause(const AudioSource& aSource)
{
	if (aSource.isMusic())
	{
		mFacade->pauseMusic();
	}
	else
	{
		throw std::logic_error("Only music sources can be paused.");
	}
}

/**
 * @brief Resume the given audio source (must be music). If the music is already playing, nothing happens.
 *
 * @param aSource The audio source to resume.
 *
 * @throw std::logic_error if the audio source is not music. (Only music sources can be resumed.)
 */
void AudioManager::resume(const AudioSource& aSource)
{
	if (aSource.isMusic())
	{
		mFacade->resumeMusic();
	}
	else
	{
		throw std::logic_error("Only music sources can be resumed.");
	}
}

/**
 * @brief Stop the audio source.
 *
 * @param aSource The audio source to stop.
 *
 * @throw std::logic_error if the audio source is not music. (Only music sources can be stopped.)
 */
void AudioManager::stop(const AudioSource& aSource)
{
	if (aSource.isMusic())
	{
		mFacade->stopMusic();
	}
	else
	{
		throw std::logic_error("Only music sources can be stopped.");
	}
}

/**
 * @brief Plays all the audio sources in the current scene, where play on wake is true
 */
void AudioManager::wake()
{
	// Iterate through all objects with audio components
	for (GameObject& gameObject : mObjects)
	{
		// Iterate through all audio sources in the object
		for (AudioSource& component : gameObject.getComponents<AudioSource>())
		{
			if (component.getPlayOnWake())
			{
				play(component);
			}
		}
	}
}

IAudioFacade& AudioManager::getFacade() { return *mFacade; }

/**
 * @brief Load the sound from the audio source into memory
 */
void AudioManager::loadSound(const AudioSource& aAudio)
{
	if (aAudio.isMusic())
	{
		mFacade->loadMusic(aAudio.getFileName());
	}
	else
	{
		mFacade->loadSound(aAudio.getFileName());
	}
}

/**
 * @brief Clear all sounds from memory
 */
void AudioManager::clearSounds() { getFacade().unloadAll(); }

/**
 * @brief Add a game object to the list of objects that have audio sources
 *
 * @param aObject The game object to add
 */
void AudioManager::addObject(GameObject& aObject)
{
	auto it = std::find_if(mObjects.begin(), mObjects.end(),
						   [&aObject](const std::reference_wrapper<GameObject>& wrapper)
						   {
							   return &wrapper.get() == &aObject; // Compare addresses
						   });
	if (it == mObjects.end())
	{
		// Object has not been added yet
		mObjects.push_back(aObject);
	}
}

/**
 * @brief Remove a game object from the list of objects that have audio sources. If the object is not in the list,
 * nothing is done.
 *
 * @param aObject The game object to remove
 *
 * @note This method does not delete the object from memory, it only removes it from the list of objects that have audio
 */
void AudioManager::removeObject(GameObject& aObject)
{
	auto it = std::remove_if(mObjects.begin(), mObjects.end(), [&aObject](const std::reference_wrapper<GameObject>& obj)
							 { return &obj.get() == &aObject; });
	if (it != mObjects.end())
	{
		mObjects.erase(it, mObjects.end());
	}
}

const std::vector<std::reference_wrapper<GameObject>>& AudioManager::getObjects() const { return mObjects; }

/**
 * @brief Clear all objects from the list of objects that have audio sources
 */
void AudioManager::clearObjects() { mObjects.clear(); }
