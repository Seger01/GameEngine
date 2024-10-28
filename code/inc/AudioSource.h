#pragma once

#include "Component.h"
#include <string>

#define DEFAULT_VOLUME 50

class AudioSource : public Component {
public:
  AudioSource(std::string aPath, bool aIsMusic = false);
  void play(bool aLooping);
  void stop();

  void setPlayOnWake(bool aValue);
  bool getPlayOnWake();
  void setLooping(bool aLooping);
  bool getLooping();
  void setVolume(unsigned aVolume);
  unsigned getVolume();
  void setXDirection(int aXDirection);
  int getXDirection();
  void setXVelocity(int aXVelocity);
  int getXVelocity();

private:
  std::string mFileName;
  bool mPlayOnAwake;
  bool mLoop;
  bool mIsMusic;

private:
  const static int mMaxVolume{100};
  const static int minXDirection{-100};
  const static int maxXDirection{100};
  const static int mMaxVelocity{10};
  int mVolume;
  int mXCoord;
  int mXVelocity;
};