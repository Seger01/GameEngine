#include "AudioSource.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include <iostream>
#include <stdexcept>

AudioSource::AudioSource(std::string aPath, bool aIsMusic, std::string aTag)
	: Component(aTag), mPlayOnAwake{false}, mLoop{false}, mIsMusic(aIsMusic), mVolume{DEFAULT_VOLUME}, mXCoord{0}
{
	mFileName = FSConverter().getResourcePath(aPath);
}

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

void AudioSource::setXDirection(int aXCoord)
{
	std::cout << "Setting X direction to " << aXCoord << std::endl;
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

void AudioSource::setXDirection(int aListenerX, int aSourceX)
{
	int xCoord = aSourceX - aListenerX;
	setXDirection(xCoord);
}

int AudioSource::getXDirection() const { return mXCoord; }

std::string AudioSource::getFileName() const { return mFileName; }

bool AudioSource::isMusic() const { return mIsMusic; }

std::unique_ptr<Component> AudioSource::clone() const { return std::make_unique<AudioSource>(*this); }