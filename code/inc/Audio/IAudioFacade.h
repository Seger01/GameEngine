#pragma once

#include <string>

class IAudioFacade
{
public:
	virtual void loadSound(const std::string& aPath) = 0;
	virtual void loadMusic(const std::string& aPath) = 0;
	virtual void unloadAll() = 0;
	virtual bool audioIsLoaded(const std::string& aPath) const = 0;
	virtual bool musicIsLoaded() const = 0;

	virtual void playSound(const std::string& aPath, bool aLooping, unsigned aVolume, int aDirection) = 0;
	virtual void playMusic(int aVolume) = 0;
	virtual void stopMusic() = 0;

	virtual bool isPlaying(const std::string& aPath) const = 0;
	virtual bool isMusicPlaying() const = 0;
};