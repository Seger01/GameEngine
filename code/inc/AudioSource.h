#pragma once

#include "Component.h"
#include <string>

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
  void setXCoord(int aXCoord);
  int getXCoord();
  void setXVelocity(int aXVelocity);
  int getXVelocity();

private:
  std::string mFileName;
  bool mPlayOnAwake;
  bool mLoop;
  bool mIsMusic;

private:
  const static int minXCoord{-100};
  const static int maxXCoord{100};
  const static int mMaxVelocity{10};
  int mVolume;
  int mXCoord;
  int mXVelocity;
};