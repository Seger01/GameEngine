#include "AudioSource.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include <iostream>
#include <stdexcept>

AudioSource::AudioSource(std::string aPath, bool aIsMusic)
    : mPlayOnAwake{false}, mLoop{false}, mIsMusic(aIsMusic), mVolume{DEFAULT_VOLUME}, mXCoord{0} {
    mFileName = FSConverter().getResourcePath(aPath);
}

void AudioSource::play(bool aLooping) {
    setLooping(aLooping);

    EngineBravo::getInstance().getAudioManager().play(*this);
}

void AudioSource::stop() {
    if (mIsMusic) {
        EngineBravo::getInstance().getAudioManager().stop(*this);
    } else {
        throw std::logic_error("Only music sources can be stopped.");
    }
}

void AudioSource::setPlayOnWake(bool aValue) { mPlayOnAwake = aValue; }

bool AudioSource::getPlayOnWake() const { return mPlayOnAwake; }

void AudioSource::setLooping(bool aLooping) { mLoop = aLooping; }

bool AudioSource::getLooping() const { return mLoop; }

void AudioSource::setVolume(unsigned aVolume) {
    if (aVolume > mMaxVolume) {
        std::cerr << "Volume cannot be greater than " << mMaxVolume << ". Setting to " << mMaxVolume << " instead\n";
        mVolume = 100;
    } else {
        mVolume = aVolume;
    }
}

unsigned AudioSource::getVolume() const { return mVolume; }

void AudioSource::setXDirection(int aXCoord) {
    if (aXCoord < minXDirection || aXCoord > maxXDirection) {
        std::cerr << "X coordinate must be between " << minXDirection << " and " << maxXDirection
                  << ". Setting to 0 instead\n";
        mXCoord = 0;
    } else {
        mXCoord = aXCoord;
    }
}

int AudioSource::getXDirection() const { return mXCoord; }

std::string AudioSource::getFileName() const { return mFileName; }

bool AudioSource::isMusic() const { return mIsMusic; }

std::unique_ptr<Component> AudioSource::clone() const { return std::make_unique<AudioSource>(*this); }