/**
 * @file AudioSource.cpp
 *
 * @brief This file contains the implementation of the AudioSource class
 */
#include "AudioSource.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include <iostream>
#include <stdexcept>

/**
 * @brief Construct a new AudioSource::AudioSource object. Sets the volume to the default value (50)
 *
 * @param aPath The path to the audio file. Must be relative to the resources folder.
 * @param aIsMusic Whether the audio source is music. Defaults to false.
 * @param aTag The tag of the audio source. Defaults to "defaultAudioSource".
 */
AudioSource::AudioSource(const std::string& aPath, bool aIsMusic, const std::string& aTag)
	: Component(aTag), mPlayOnAwake{false}, mLoop{false}, mIsMusic(aIsMusic), mVolume{DEFAULT_VOLUME}, mXCoord{0}
{
	mFileName = FSConverter().getResourcePath(aPath);
}

/**
 * @brief Play this audio source
 *
 * @param aLooping Whether the audio source should loop. Defaults to false.
 */
void AudioSource::play(bool aLooping)
{
	setLooping(aLooping);

	EngineBravo::getInstance().getAudioManager().play(*this);
}

/**
 * @brief Stop playing this audio source
 *
 * @note Does not check if this is a music source (only music sources can be stopped), because this is the
 * responsibility of the AudioManager
 */
void AudioSource::stop() { EngineBravo::getInstance().getAudioManager().stop(*this); }

void AudioSource::setPlayOnWake(bool aValue) { mPlayOnAwake = aValue; }

bool AudioSource::getPlayOnWake() const { return mPlayOnAwake; }

void AudioSource::setLooping(bool aLooping) { mLoop = aLooping; }

bool AudioSource::getLooping() const { return mLoop; }

void AudioSource::setVolume(unsigned aVolume)
{
	if (aVolume > mMaxVolume)
	{
		std::cerr << "Volume cannot be greater than " << mMaxVolume << ". Setting to " << mMaxVolume << " instead\n";
		mVolume = 100;
	}
	else
	{
		mVolume = aVolume;
	}
}

unsigned AudioSource::getVolume() const { return mVolume; }

/**
 * @brief Set the direction of the audio source
 *
 * @param aXCoord The direction of the audio source. Must be between mMinXDirection and mMaxXDirection
 */
void AudioSource::setXDirection(int aXCoord)
{
	if (aXCoord < mMinXDirection)
	{
		std::cerr << "X coordinate must be greater than or equal to " << mMinXDirection << ". Setting to "
				  << mMinXDirection << " instead\n";
		mXCoord = mMinXDirection;
	}
	else if (aXCoord > mMaxXDirection)
	{
		std::cerr << "X coordinate must be less than or equal to " << mMaxXDirection << ". Setting to "
				  << mMaxXDirection << " instead\n";
		mXCoord = mMaxXDirection;
	}
	else
	{
		mXCoord = aXCoord;
	}
}

/**
 * @brief Set the direction of the audio source
 *
 * @param aListenerX The x coordinate of the listener
 * @param aSourceX The x coordinate of the source
 */
void AudioSource::setXDirection(int aListenerX, int aSourceX)
{
	int xCoord = aSourceX - aListenerX;
	setXDirection(xCoord);
}

int AudioSource::getXDirection() const { return mXCoord; }

std::string AudioSource::getFileName() const { return mFileName; }

bool AudioSource::isMusic() const { return mIsMusic; }

std::unique_ptr<Component> AudioSource::clone() const { return std::make_unique<AudioSource>(*this); }