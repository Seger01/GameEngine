/**
 * @file MixerFacade.h
 *
 * @brief This file contains the declaration of the MixerFacade class
 */
#pragma once

#include "Audio/IAudioFacade.h"
#include "Audio/MixerContainer.h"
#include <soloud.h>
#include <string>

/**
 * @class MixerFacade
 *
 * @brief Implements IAudioFacade, using SoLoud for all audio operations.
 */
class MixerFacade : public IAudioFacade
{
public:
	MixerFacade();
	virtual ~MixerFacade();

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

	bool isPlaying(const std::string& aPath) override;
	bool isMusicPlaying() override;

	int distanceToAngle(int aDirection) const;

private:
	SoLoud::Soloud mEngine;
	MixerContainer mMixerContainer;
	std::unordered_map<std::string, SoLoud::handle> mSoundHandles;
	SoLoud::handle mMusicHandle;
};
