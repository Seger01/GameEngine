#pragma once

#include <string>

class IAudioFacade {
public:
    virtual void addSound(std::string aPath) = 0;
    virtual void playSound(std::string aPath, bool aLooping, unsigned aVolume, int aDirection) = 0;
    virtual void addMusic(std::string aPath) = 0;
    virtual void playMusic(int aVolume) = 0;
    virtual void stopMusic() = 0;
    virtual bool isPlaying(const std::string& aPath) const = 0;
};