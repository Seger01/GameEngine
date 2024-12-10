/**
 * @file AudioSource.h
 *
 * @brief This file contains the declaration of the AudioSource class
 */
#pragma once

#include "Component.h"
#include <string>

#define DEFAULT_VOLUME 50

/**
 * @class AudioSource
 *
 * @brief This class is a component, responsible for holding the data of an audio source
 */
class AudioSource : public Component
{
public:
	AudioSource(const std::string& aPath, bool aIsMusic = false, const std::string& aTag = "defaultAudioSource");
	void play(bool aLooping = false);
	void stop();

	bool isMusic() const;
	void setPlayOnWake(bool aValue);
	bool getPlayOnWake() const;
	void setLooping(bool aLooping);
	bool getLooping() const;
	void setVolume(unsigned aVolume);
	unsigned getVolume() const;
	void setXDirection(int aListenerX, int aSourceX);
	void setXDirection(int aXDirection);
	int getXDirection() const;
	std::string getFileName() const;
	std::unique_ptr<Component> clone() const override;

private:
	/// @brief The absolute path to the audio file.
	std::string mFileName;
	/// @brief Whether the audio source should play on awake (when the scene is loaded)
	bool mPlayOnAwake;
	/// @brief Whether the audio source should loop
	bool mLoop;
	/// @brief Whether the audio source is music
	bool mIsMusic;

private:
	/// @brief The maximum volume of the audio source
	const static int mMaxVolume{100};
	/// @brief The minimum direction of the audio source (how far it can be placed to the left)
	const static int mMinXDirection{-100};
	/// @brief The maximum direction of the audio source (how far it can be placed to the right)
	const static int mMaxXDirection{100};
	/// @brief The volume of the audio source
	int mVolume;
	/// @brief The direction of the audio source (0 is centre, lt 0 is left, gt 0 is right)
	int mXCoord;
};