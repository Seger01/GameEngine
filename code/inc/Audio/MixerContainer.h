/**
 * @file MixerContainer.h
 *
 * @brief This file contains the declaration of the MixerContainer class
 */
#pragma once

#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include <string>
#include <unordered_map>

/**
 * @class MixerContainer
 *
 * @brief Owns SoLoud sound effects (Wav) and music (WavStream), ensuring unique load per sound.
 */
class MixerContainer
{
public:
	MixerContainer();
	virtual ~MixerContainer();
	MixerContainer(const MixerContainer&);
	MixerContainer(MixerContainer&&);
	MixerContainer& operator=(const MixerContainer&);
	MixerContainer& operator=(MixerContainer&&);

	void addSound(const std::string& aPath, SoLoud::Wav* aSound);
	SoLoud::Wav* getSound(const std::string& aIndex);
	const SoLoud::Wav* getSound(const std::string& aIndex) const;

	void addMusic(const std::string& aPath, SoLoud::WavStream* aMusic);
	SoLoud::WavStream* getMusic();
	const SoLoud::WavStream* getMusic() const;

	void clear();

private:
	std::unordered_map<std::string, SoLoud::Wav*> mSfx;
	std::string mMusicPath;
	SoLoud::WavStream* mMusic;
};
