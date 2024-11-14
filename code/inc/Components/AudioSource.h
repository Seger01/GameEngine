#pragma once

#include "Component.h"
#include <string>

#define DEFAULT_VOLUME 50

class AudioSource : public Component {
public:
    AudioSource(std::string aPath, bool aIsMusic = false);
    void play(bool aLooping);
    void stop();

    bool isMusic() const;
    void setPlayOnWake(bool aValue);
    bool getPlayOnWake() const;
    void setLooping(bool aLooping);
    bool getLooping() const;
    void setVolume(unsigned aVolume);
    unsigned getVolume() const;
    void setXDirection(int aXDirection);
    int getXDirection() const;
    std::string getFileName() const;
    std::unique_ptr<Component> clone() const override;

private:
    std::string mFileName;
    bool mPlayOnAwake;
    bool mLoop;
    bool mIsMusic;

private:
    const static int mMaxVolume{100};
    const static int minXDirection{-100};
    const static int maxXDirection{100};
    int mVolume;
    int mXCoord;
};