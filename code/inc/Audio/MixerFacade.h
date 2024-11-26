#pragma once

#include "IAudioFacade.h"
#include "MixerContainer.h"
#include <SDL_mixer.h>

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
	void stopMusic() override;

	bool isPlaying(const std::string& aPath) const override;
	bool isMusicPlaying() const override;

	int distanceToAngle(int aDirection) const;
	int findAvailableChannel();

private:
	const unsigned mChannelCount;
	unsigned mLastUsedChannel; // to help with finding the next available channel
	MixerContainer mMixerContainer;
};